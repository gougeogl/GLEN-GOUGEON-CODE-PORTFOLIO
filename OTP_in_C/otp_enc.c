/******************************************************************
* FileName: otp_enc.c
* Author:	Glen Gougeon
* Date:		7-28-20
* Last MOD:	7-28-20

* Description:	OTP Client-Server Project

	This is the encryption CLIENT program
	that takes arguments:
	[otp_enc] [plaintext] [key] [port]

	---------------------------------------
	*NOTE* otp_enc_d is the server/'daemon'
	---------------------------------------

	PseudoCode/Steps:
	1.  Setup address structs per manpages
	2.  Create socket
	3.  Connect to otp_enc_d via [port]
	4.  Authenticate server connection
	5.  Read [plaintext] to a buffer
	6.  Read [key] to a different buffer
	7.  VALIDATE that LENGTH [plaintext] == [key] 
	8.  send [plaintext] size to otp_enc_d
	9.  send [plaintext]
	10. send [key] size to otp_enc_d
	11. send [key]
	12. Receive new ciphertext from otp_enc_d
	13. print new ciphertext
	14. free buffers
	15. close socket

******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "helpers.h"

int main(int argc, char *argv[])
{
	// STEP 1 SETUP STRUCTS
	int socketFD, portNumber; 
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;

	if (argc < 3) { fprintf(stderr,"USAGE: %s plaintext key port\n", argv[0]); exit(0); } // Check usage & args

	// Set up the server address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[3]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverHostInfo = gethostbyname("localhost"); // Convert the machine name into a special form of address
	if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr_list[0], serverHostInfo->h_length); // Copy in the address

	// STEP 2: SET UP THE SOCKET
	socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (socketFD < 0) error("CLIENT: ERROR opening socket");
	
	// STEP 3: CONNECT TO SERVER
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
		error("CLIENT: ERROR connecting");

	// STEP 4: VALIDATE SERVER fd, port, enum Cipher flag
	authenticateServer(socketFD, argv[3], ENC);

	// STEP 5: READ PLAINTEXT
	//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
	int bufferSize = 0;
	char* buffer = writeFileToBuffer(argv[1], &bufferSize);
	if (!isValidValue(buffer, bufferSize)) { fprintf(stderr, "otp_enc error: input contains bad characters\n"); exit(1); }
	//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

	// STEP 6: READ KEY
	int keySize = 0;
	char* key = writeFileToBuffer(argv[2], &keySize);
	if (!isValidValue(key, keySize)) { fprintf(stderr, "otp_enc error: input contains bad characters\n"); exit(1); }
	//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

	// STEP 7: VALIDATE KEY LENGTH
	if (keySize < bufferSize) { fprintf(stderr, "Your key \'%s\' is too short\n", argv[2]); exit(1); }

	// STEP 8: SEND [plaintext] SIZE
	int expectedSize = sendSize(socketFD, &bufferSize);

	// STEP 9: SEND [plaintext] 
	if(sendall(socketFD, buffer, &expectedSize) < 0) error("CLIENT: writing PTXT to socket");

	// STEP 10: SEND [key] SIZE
	int expectedKeySize = sendSize(socketFD, &keySize);

	// STEP 11: SEND [key]
	if (sendall(socketFD, key, &expectedKeySize) < 0) error("CLIENT: writing KEY to socket");

	// STEP 12: RECEIVE [ciphertext]
	char* ctxt = recvAll(socketFD, &expectedSize);
	if (ctxt == NULL) error("CLIENT: reading CTXT from socket");

	// STEP 13: PRINT [ciphertext]
	printf("%s\n", ctxt);
	fflush(stdout);

	// STEP 14: FREE BUFFERS
	free(buffer);
	buffer = NULL;
	free(key);
	key = NULL;
	free(ctxt);
	ctxt = NULL;

	// STEP 15: CLOSE SOCKET
	close(socketFD);
	return 0;
}
