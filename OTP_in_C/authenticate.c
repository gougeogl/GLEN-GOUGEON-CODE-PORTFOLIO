/****************************************************************
* FileName: authenticate.c
* Author:	Glen Gougeon
* Date:		7-28-20
* Last MOD:	7-28-20
* Class:	CS344 Operating Systems I

* Description:	OTP Client-Server Project
*****************************************************************/
#include "helpers.h"

// Error functions used for reporting issues
void error(const char *msg) { fprintf(stderr, "ERROR: %s\n", msg); fflush(stderr);  exit(1); }
void errorNoExit(const char* msg) { fprintf(stderr, "ERROR: %s\n", msg); fflush(stderr); }

/********************************************************************
* Function: authenticateServer()

* Dependencies: validateClient() in opposite server/'daemon' program

* Parameters:
	1. int fd. The file descriptor of established connection
	2. char* port. The port the user is expecting to connect to
	3. enum Cipher flag. Indicates Encryption/Decryption usage

* Description:
	Checks that THIS client can connect to the correct program
	on the specified [port]. For example:
		otp_enc CANNOT connect to otp_dec_d and vice versa

* Returns:
	1. If the incoming connection matches the name of this program
	   and is the same port THIS program is connected to

	0. Otherwise

*********************************************************************/
void authenticateServer(int fd, char* port, enum Cipher flag)
{
	char* is_ENC_D = "otp_enc_d";
	char* is_DEC_D = "otp_dec_d";

	// allocate local buffer
	int originalSize = MAX_BUFFER * sizeof(char);
	char original[originalSize];
	memset(original, '\0', originalSize);

	// allocate local buffer
	int bufferSize = MAX_BUFFER * sizeof(char);
	char buffer[bufferSize];
	memset(buffer, '\0', bufferSize);

	char* server = NULL;
	// copy program name to buffer based on flag value
	switch (flag)
	{
		case ENC:
			strncpy(buffer, is_ENC_D, strlen(is_ENC_D));
			server = is_ENC_D;
			break;

		case DEC:
			strncpy(buffer, is_DEC_D, strlen(is_DEC_D));
			server = is_DEC_D;
			break;
	}

	// then concatenate port entered onto buffer
	strncat(buffer, port, strlen(port));

	// save it as 'original' for comparison after 
	// server responds
	strncpy(original, buffer, strlen(buffer));

	// outgoing
	int charsWritten = send(fd, buffer, strlen(buffer), 0); // Write REQUEST_ID to the server
	if (charsWritten < 0) errorNoExit("CLIENT: could not send data for authentication");
	if (charsWritten < strlen(buffer)) errorNoExit("WARNING: CLIENT: Not all data written to socket!");

	memset(buffer, '\0', bufferSize);

	// incoming
	int charsRead = recv(fd, buffer, bufferSize - 1, 0); // Write to the server
	if (charsRead < 0) errorNoExit("CLIENT: nothing received for authentication");
	if (charsRead < strlen(buffer)) errorNoExit("CLIENT: Not all data read for authentication!");

	if (strncmp(buffer, original, strlen(original)) != 0)
	{
		fprintf(stderr, "Error: could not contact %s on port %s\n", server, port);
		fflush(stderr);
		exit(2);
	}
}

/********************************************************************
* Function: authenticateClient()

* Dependencies: validateClient() in opposite server/'daemon' program

* Parameters:
	1. int fd. The file descriptor of established connection
	2. char* host. Not exactly. The name of THIS program a.k.a.
	   argv[0]
	3. char* port. The port the user is expecting to connect to

* Description:
	Used to ensure THIS server is being contacted by the
	correct client program by checking if the [programName||port]
	matches argv[0]||port] HERE.

* Returns:
	1. If the incoming connection matches the name of this program
	   and is the same port THIS program is connected to

	0. Otherwise

*********************************************************************/
int authenticateClient(int fd, char* host, char* port)
{
	int isValidClient = 0;

	// allocate local buffer
	int bufferSize = MAX_BUFFER * sizeof(char);
	char buffer[bufferSize];
	memset(buffer, '\0', bufferSize);

	// allocate local buffer
	int tempSize = MAX_BUFFER * sizeof(char);
	char temp[tempSize];
	memset(temp, '\0', tempSize);

	strncpy(temp, host, strlen(host));
	strncat(temp, port, strlen(port));

	// incoming
	int charsRead = recv(fd, buffer, bufferSize - 1, 0); // Write to the server
	if (charsRead < 0) errorNoExit("SERVER: nothing received for authentication");
	if (charsRead < strlen(buffer)) errorNoExit("SERVER: Not all data read for authentication!"); 

	if (strncmp(temp, buffer, strlen(temp)) == 0) { isValidClient = 1; }

	// outgoing
	int charsWritten = send(fd, temp, strlen(temp), 0); // Write REQUEST_ID to the server
	if (charsWritten < 0) errorNoExit("SERVER: validateServer failure\n");
	if (charsWritten < strlen(buffer)) errorNoExit("SERVER: validateServer : Not all data written to socket!"); 

	return isValidClient;
}


