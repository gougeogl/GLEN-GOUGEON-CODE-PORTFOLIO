/******************************************************************
* FileName: otp_dec_d.c
* Author:	Glen Gougeon
* Date:		7-28-20
* Last MOD:	7-28-20

* Description:	OTP Client-Server Project

	This is the decryption 'daemon' (not a true daemon program)
	that takes arguments:
	[otp_enc_d] [port] (should run in background so.. use [&])

	PseudoCode/Steps:
	1. Setup address structs per manpages
	2. Create socket for THIS daemon/server
	3. Bind the socket to the [port] argument
	4. Listen on socket for ANY connection
	5. Authenticate client is allowed
	6. if true: Fork a new process
	7. Validate incoming ciphertext size
	8. Receive new ciphertext
	9. Validate incoming key size
	10 Receive new key
	11 Decrypt and create plaintext
	12 return new plaintext
	13 free buffers
	14 repeat

******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include "helpers.h"

void decrypt(char* CIPHERTEXT, int ctxt_Length, char* KEY, int key_Length);

int main(int argc, char *argv[])
{
	// STEP 1: SETUP ADDRESS STRUCTS
	int listenSocketFD, establishedConnectionFD, portNumber; //charsRead;
	socklen_t sizeOfClientInfo;
	//char buffer[256];
	struct sockaddr_in serverAddress, clientAddress;

	if (argc < 2) { fprintf(stderr,"USAGE: %s port\n", argv[0]); exit(1); } // Check usage & args

	// Set up the address struct for this process (the server)
	memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[1]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

	// STEP 2: SETUP SOCKET
	listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (listenSocketFD < 0) error("ERROR opening socket");

	// STEP 3: BIND SOCKET TO [port]
	if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to port
		error("ERROR on binding");

	// STEP 4: LISTEN ON SOCKET FOR ANY CONNECTION
	listen(listenSocketFD, 5); // Flip the socket on - it can now receive up to 5 connections

	while (1) // STEP 14: REPEAT
	{
		// Accept a connection, blocking if one is not available until one connects
		sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
		establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
		if (establishedConnectionFD < 0) errorNoExit("on accept");

		// STEP 5: AUTHENTICATE CLIENT
		if (authenticateClient(establishedConnectionFD, argv[0], argv[1]))
		{
			// STEP 6: FORK ETC
			int childStatus = -5; // -5 to be distinct
			int childPid = fork();
			switch (childPid)
			{
				case -1: errorNoExit("otp_dec_d BAD FORK!"); break;

				case 0:
				{
					// STEP 7: VALIDATE CTXT SIZE
					int expectedSize = recvSize(establishedConnectionFD);

					// STEP 8: RECEIVE CTXT
					char* buffer = recvAll(establishedConnectionFD, &expectedSize);
					if (buffer == NULL) errorNoExit("SERVER: reading PTXT from socket");

					// STEP 9: VALIDATE KEY SIZE
					int expectedKeySize = recvSize(establishedConnectionFD);

					// STEP 10: RECEIVE KEY
					char* key = recvAll(establishedConnectionFD, &expectedKeySize);
					if (key == NULL) errorNoExit("SERVER: reading KEY from socket");

					// STEP 11: DECRYPTION
					decrypt(buffer, expectedSize, key, expectedKeySize);

					// STEP 12: RETURN NEW PLAINTEXT
					if (sendall(establishedConnectionFD, buffer, &expectedSize) < 0)
					{
						errorNoExit("SERVER: ERROR writing PTXT to socket");
					}

					// STEP 13: FREE BUFFERS
					free(buffer);
					buffer = NULL;
					free(key);
					key = NULL;

					exit(0);
					break;
				}
				default:
				{
					childPid = waitpid(childPid, &childStatus, 0);
				}
			}
		}
	}
	close(establishedConnectionFD); // Close the existing socket which is connected to the client
	close(listenSocketFD); // Close the listening socket
	return 0; 
}

/***********************************************************************
* Function: decrypt()
* Parameters:
	1. char* CIPHERTEXT. The Ciphertext to Decrypt
	2. int ctxt_length. The length in bytes of CIPHERTEXT as an integer
	3. char* KEY. The key used for to Decrypt
	4. int key_length. The length in bytes of KEY as an integer

* Description:
	Mimicks One-Time-Pad (O.T.P.) decryption scheme using capitol
	letters of English alphabet as [0-25] and SPACE [26] for
	ciphertext (ctxt) and key values.

	PROCESS: USE MODULAR 27 SUBTRACTION
		For each respective char of ptxt and key do:
			ptxt[index] = ctxt[index] - key[index]
			if(ptxt[index] < 0)
				then ptxt[index] += 27
				then MOD 27 to get the result
			else
				then MOD 27 to get the result

* RESULT:
	converts CIPHERTEXT --> PLAINTEXT
	KEY is NOT affected.

************************************************************************/
void decrypt(char* CIPHERTEXT, int ctxt_Length, char* KEY, int key_Length)
{
	if (key_Length >= ctxt_Length)
	{
		int ctVal = 0;
		int kVal = 0;
		int diff = 0;

		int i;
		for (i = 0; i < ctxt_Length; i++)
		{
			// cast into ascii equivalent
			ctVal = (int)CIPHERTEXT[i];
			if (ctVal != 0) // ignore newline a.k.a. zero
			{
				// ciphertext
				if (ctVal != 32) { ctVal -= 65; }
				else { ctVal -= 6; }

				// key
				kVal = (int)KEY[i];
				if (kVal != 32) { kVal -= 65; }
				else { kVal -= 6; }

				// ciphertext[index] - key[index]
				diff = ctVal -= kVal;

				// if negative, then add 27 instead
				if (diff < 0)
				{
					diff += 27;
				}

				// MOD 27
				diff = diff % 27; 

				// if SPACE (26) then add 6
				if (diff == 26)
				{
					diff += 6;
				}
				else { diff += 65; }

				// overwrite with new value
				CIPHERTEXT[i] = (char)diff;
			}

		} // END for-loop
		CIPHERTEXT[i] = '\0';
	}
	else { errorNoExit("otp_dec_d your key is too short"); }
}

