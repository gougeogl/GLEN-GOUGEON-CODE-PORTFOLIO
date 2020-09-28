/******************************************************************
* FileName: otp_enc_d.c
* Author:	Glen Gougeon
* Date:		7-28-20
* Last MOD:	7-28-20
* Class:	CS344 Operating Systems I

* Description:	OTP Client-Server Project

	This is the encryption 'daemon' (not a true daemon program)
	that takes arguments:
	[otp_enc_d] [port] (should run in background so.. use [&])

	PseudoCode/Steps:
	1. Setup address structs per manpages
	2. Create socket for THIS daemon/server
	3. Bind the socket to the [port] argument
	4. Listen on socket for ANY connection
	5. Authenticate client is allowed
	6. if true: Fork a new process
	7. Validate incoming plaintext size
	8. Receive new plaintext
	9. Validate incoming key size
	10 Receive new key
	11 Encrypt and create ciphertext
	12 return new ciphertext
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

void encrypt(char* PLAINTEXT, int pt_length, char* KEY, int key_length);

int main(int argc, char *argv[])
{
	// STEP 1: SETUP ADDRESS STRUCTS
	int listenSocketFD, establishedConnectionFD, portNumber;
	socklen_t sizeOfClientInfo;
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

	// STEP 3: BIND SOCKET TO [port] (Enable the socket to begin listening)
	if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
		error("ERROR on binding");

	// STEP 4: LISTEN ON SOCKET FOR ANY CONNECTION
	listen(listenSocketFD, 5); // Flip the socket on - it can now receive up to 5 connections

	while (1) // STEP 14: REPEAT
	{
		// Accept a connection, blocking if one is not available until one connects
		sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
		establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
		if (establishedConnectionFD < 0) errorNoExit("accept failure");

		// STEP 5: VALIDATE CLIENT
		if (authenticateClient(establishedConnectionFD, argv[0], argv[1]))
		{
			// STEP 6: FORK ETC
			int childStatus = -5; // -5 to be distinct
			int childPid = fork();
			switch (childPid)
			{
				case -1: errorNoExit("otp_enc_d BAD FORK!"); break;

				case 0:
				{
					// STEP 7: VALIDATE PTXT SIZE
					int expectedSize = recvSize(establishedConnectionFD);

					// STEP 8: RECEIVE PTXT
					char* buffer = recvAll(establishedConnectionFD, &expectedSize);
					if (buffer == NULL) errorNoExit("SERVER: ERROR reading PTXT from socket");

					// STEP 9: VALIDATE KEY SIZE
					int expectedKeySize = recvSize(establishedConnectionFD);

					// STEP 10: RECEIVE KEY
					char* key = recvAll(establishedConnectionFD, &expectedKeySize);
					if (key == NULL) errorNoExit("SERVER: ERROR reading KEY from socket"); 

					// STEP 11: ENCRYPTION
					encrypt(buffer, expectedSize, key, expectedKeySize);

					// STEP 12: RETURN NEW CIPHERTEXT
					if (sendall(establishedConnectionFD, buffer, &expectedSize) < 0)
					{
						errorNoExit("SERVER: ERROR writing CTXT to socket");
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
* Function: encrypt()
* Parameters:
	1. char* PLAINTEXT. The Plaintext to Encrypt
	2. int pt_length. The length in bytes of PLAINTEXT as an integer
	3. char* KEY. The key used for to Encrypt
	4. int key_length. The length in bytes of KEY as an integer

* Description:
	Mimicks One-Time-Pad (O.T.P.) encryption scheme using capitol
	letters of English alphabet as [0-25] and SPACE [26] for
	plaintext (ptxt) and key values.

	PROCESS: USE MODULAR 27 ADDITION
		For each respective char of ptxt and key do:
			ctxt[index] = ptxt[index] + key[index] MOD 27

* RESULT:
	converts PLAINTEXT --> CIPHERTEXT
	KEY is NOT affected.

************************************************************************/
void encrypt(char* PLAINTEXT, int pt_length, char* KEY, int key_length)
{
	if (key_length >= pt_length)
	{
		int ptVal = 0;
		int kVal = 0;
		int sum = 0;

		int i;
		for (i = 0; i < pt_length; i++)
		{
			ptVal = (int)PLAINTEXT[i];
			if (ptVal != 0) // ignore newline a.k.a. zero
			{
				if (ptVal != 32) { ptVal -= 65; }
				else { ptVal -= 6; }

				kVal = (int)KEY[i];
				if (kVal != 32) { kVal -= 65; }
				else { kVal -= 6; }

				sum = ptVal += kVal;

				sum = sum % 27; // message + key (mod 27) 

				if (sum == 26)
				{
					sum += 6;
				}
				else { sum += 65; }

				PLAINTEXT[i] = (char)sum;
			}
		} // END for-loop
		PLAINTEXT[i] = '\0';
	}
	else
	{
		fprintf(stderr, "%s\n", "otp_enc_d your key is too short");
		fflush(stdout);
	}
}
