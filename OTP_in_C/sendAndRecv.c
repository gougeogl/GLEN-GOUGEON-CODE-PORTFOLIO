/************************************************************
* FileName sendAndRecv.c
* Author:	Glen Gougeon
* Date:		7-23-20
* Last MOD:	7-28-20

* Description:
	OTP encryption/decryption Client-Server Program:

	Helpers that use send() and recv() library functions. 
	Architecture implements sending sizes ahead of 
	messages instead of concatenation of data in clients
	and parsing on servers. 

************************************************************/
#include "helpers.h"

/************************************************************
* Function: sendSize()
* Parameters:
	1. int fd. The file descriptor of connected socket
	           to send the file size to.
	2. int* The size of the prospectus file

* Description:
	Sends the size of a buffer after a call to
	writeFileToBuffer() [see description] so that the 
	other connection knows when they have received the
	entire buffer and can stop.

* Returns:
	0:  success
	-1: error

*************************************************************/
int sendSize(int fd, int* bytesToSend)
{
	int status = 0;

	// allocate buffer
	char* buffer = _initBuffer(6);
	int bufferSize = sizeof(buffer);

	snprintf(buffer, bufferSize, "%d", *bytesToSend);

	// outgoing
	int charsWritten = send(fd, buffer, strlen(buffer), 0); // Write to the server
	if (charsWritten < 0) error("sending file size");
	if (charsWritten < strlen(buffer)) error("Not all file size data written to socket!"); 

	memset(buffer, '\0', bufferSize);

	// incoming
	int charsRead = recv(fd, buffer, bufferSize - 1, 0); // Write to the server
	if (charsRead < 0) perror("recvSize");
	if (charsRead < strlen(buffer)) perror("SEND SIZE: Not all data read from socket!\n");

	if (charsWritten != -1)
	{
		// doing this will return how many sent
		status = atoi(buffer);
	}

	// clean-up buffer
	free(buffer);
	buffer = NULL;

	return status;
}

/************************************************************
* Function: recvSize()
* Parameter: 
	int fd. File descriptor of currently connected socket

* Description:
	Works in conjunction with sendSize() to establish
	expected size of message being sent over the wire.

* Returns:
	size of the expected message as an integer -OR-
	-1 on error

*************************************************************/
int recvSize(int fd)
{
	int status = 0;

	// allocate buffer
	char* buffer = _initBuffer(6);
	int bufferSize = sizeof(buffer);

	// incoming
	int charsRead = recv(fd, buffer, bufferSize - 1, 0); // Write to the server
	if (charsRead < 0) errorNoExit("Nothing received from socket"); 
	if (charsRead < strlen(buffer)) errorNoExit("Not all data received from socket!"); 

	// outgoing
	int charsWritten = send(fd, buffer, strlen(buffer), 0); // Write to the server
	if (charsWritten < 0) perror("sendSize");
	if (charsWritten < strlen(buffer)) perror("RECV SIZE: Not all data written to socket!\n");

	if (charsRead != -1)
	{
		status = atoi(buffer);
	}

	// clean-up buffer
	free(buffer);
	buffer = NULL;

	return status;
}

/****************************************************************
* Function: sendall()
* Description:
	Helper to otp_enc_d & otp_enc in client-server otp program.
	Attempts to repeatedly send a char* message (msg) to the
	opposite host. If send returns -1 (err) an err message is
	printed, and the status returned is -1.

* Parameters:
	1. int fd. The file descriptor of a connected socket.
	2. char* msg. The string message to send over the network.
	4. size_t len. The length of (msg)

* Effect:
	Entire msg successfully sent over TCP socket network OR
	an error message is printed.

* Returns:
	success: 0 (default)
	failure: -1

*****************************************************************/
int sendall(int fd, char* msg, int* len)
{
	int status = 0; // returns this value

	int totalSent = 0;
	int bytesLeft = *len;
	int charsWritten = 0;

	while (totalSent < *len)
	{
		charsWritten = send(fd, msg + totalSent, bytesLeft, 0);
		if (charsWritten < 0) { break; }
		totalSent += charsWritten;
		bytesLeft -= charsWritten;
	}

	*len = totalSent;

	if (charsWritten == -1)
	{
		errorNoExit("No data written to socket!");
		status = -1;
	}
	return status;
}

/********************************************************************
* Function: receiveAll()
* Parameters:
	1. int fd. The file descriptor of the connected socket
	2. int* len. Pointer to the expected length of the file
	   coming across the wire

* Description:
	Master recv() function.
	Looping wrapper for the recv() library function to ensure all
	data is received. -1 err from recv() are sent to stderr.
	Dynamically allocates a buffer which is returned on successful
	transfer of all data.

* Returns:
	Pointer to dynamically allocated buffer of received contents

* Post-Conditions:
	A pointer assigned to the output of this function MUST
	be freed by use of free()

*********************************************************************/
char* recvAll(int fd, int* len)
{
	// ensure file starts at beginning
	lseek(fd, 0, SEEK_SET);

	// DYNAMIC BUFFER SETUP
	char* buffer = _initBuffer(*len);

	int totalSent = 0;
	int charsRead = 0;
	int bytesLeft = *len;

	while (totalSent < *len)
	{
		charsRead = recv(fd, buffer + totalSent, bytesLeft, 0);
		if (charsRead < 0) { break; }
		totalSent += charsRead;
		bytesLeft -= charsRead;
	}

	if (charsRead == -1) { errorNoExit("No data received from socket!"); }

	return buffer;
}

