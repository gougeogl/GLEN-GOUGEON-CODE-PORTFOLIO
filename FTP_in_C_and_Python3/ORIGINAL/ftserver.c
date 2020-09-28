/****************************************************************************************
* FileName: ftserver.c
* LANGUAGE : C-99
* AUTHOR : Glen Gougeon
* CLASS : CS372 Networking
* DUE DATE :	3 - 8 - 2020
* LAST - MOD:	3 - 8 - 2020
*
* DESCRIPTION : ftserver.c : Program 2
	Server program for File Transfer Protocol - like program.
	Used in conjunction with ftclient.py to transfer files to client.
	
* CREDITS (code):
	Prof. Benjamin Brewster's Lectures, Slides, and sample code used
	as base-point for simple client-server connections in C. The error
	function is his, and some of the formatting of the send, recv calls
	are very similar.

			|*Note* --------------------------------------------------------|
			|I have re-used code I designed for different projects in CS344,|
			|namely findFileInDirectory was updated and simplified portion  |
			|of the build-rooms game, and tokenize was part of the smallsh.c|
			|program. See function descriptions for more details            |
			|---------------------------------------------------------------|
			
	** HOW TO RUN :

	 1. COMPILE: gcc -o ftserver ftserver.c
	 2. RUN:	 ftserver <PORT_NUM>, where PORT_NUM is the port to bind to

    ** HOW TO QUIT :

	 1. enter on keyboard: control-C

*****************************************************************************************/   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <assert.h> // only used by destroyTokens

// DIRECTORIES RELATED
//#include <sys/stat.h> 
#include <dirent.h> 
#include <fcntl.h>

void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues
char**  tokenize(char*);
void destroyTokens(char** tokenArr);

// FILE LOCATION PROTO-TYPES
void getFilesList(DIR* dirToSearch, char* list);
char* findFileInDirectory(DIR* dirToSearch, char* targetFileName);

// helper
int getFileSize(int* fd, char* file);
void getFlipAlias(char* longName, char* alias);
void printGetRequestStatus(char* file, char* alias, int dataPort, int flag);

int main(int argc, char *argv[])
{
	if (argc < 2) { fprintf(stderr, "USAGE: %s port\n", argv[0]); exit(1); } // Check usage & args

	int listenSocketFD, establishedConnectionFD, portNumber, charsRead, totalRead, charsSent, totalSent;
	socklen_t sizeOfClientInfo;
	char buffer[256];
	struct sockaddr_in serverAddress, clientAddress;

	// Set up the address struct for this process (the server)
	memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[1]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

	// Set up the socket : CONTROL CHANNEL
	listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (listenSocketFD < 0) error("SERVER: ERROR opening socket");

	// Enable the socket to begin listening
	if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to port
		error("SERVER: ERROR on binding");
	listen(listenSocketFD, 2); // Flip the socket on - it can now receive up to 2 connections

	printf("\nServer open on %d\n\n", portNumber);
	while (1)
	{
		// Accept a connection, blocking if one is not available until one connects
		sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
		establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
		if (establishedConnectionFD < 0) error("SERVER: ERROR on accept");

		// Get the message from the client and display it
		memset(buffer, '\0', 256);
		charsRead = recv(establishedConnectionFD, buffer, 255, 0); // Read the client's message from the socket
		if (charsRead < 0) error("SERVER: ERROR reading from socket");

		// break up cmd-line args from client for various uses
		char** client_args;
		client_args = tokenize(buffer);
		
		/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
		    		CMD OPTIONS: 
						a) LIST files
							create a list of files in current directory
							send list to client
								return err msg if not found

						b) GET file
							1. locate file in current directory
							2. determine file's size
							3. read contents into dynamic buffer
							4. send dynamic buffer contents to client

		MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
		const char* NO_FILE_ERR = "File not found.";
		int FOUND_FILE_FLAG = 0; // controls print statements
		
		int dataPortNumber = -1;

		char dataBuffer[8192];
		memset(dataBuffer, '\0', sizeof(dataBuffer));
		char readBuffer[512];

		// to be dynamically-allocated
		char* dynBuffer = NULL;
	
		/* req'd for file access in directories */
		char* fileToSend = NULL; 
		DIR* localDir;
		localDir = opendir(".");

		/* determine cmd used */
		if (strcmp(client_args[2], "LIST") == 0) // IF DIRECTORY STRUCTURE
		{
			// get list of files
			getFilesList(localDir, dataBuffer);

			// Get the data port number, convert to an integer from a string
			dataPortNumber = atoi(client_args[3]);

		}
		else if (strcmp(client_args[2], "GET") == 0) // IF FILE REQUESTED
		{
			// LOCATE FILE 
			fileToSend = findFileInDirectory(localDir, client_args[3]);

			int file_FD = 0;
			int totalFileSize = 0;

			if (fileToSend != NULL) // FILE EXISTS **
			{
				// determine file size
				totalFileSize = getFileSize(&file_FD, fileToSend);
				
				// allocate dynamic buffer for large file sizes
				dynBuffer = malloc(totalFileSize * sizeof(char));
				assert(dynBuffer != NULL);

				// Client selected file for opening
				file_FD = open(fileToSend, O_RDONLY | S_IRUSR);

				// READ FILE CONTENTS INTO dynBuffer
				totalRead = 0;
				while (totalRead < totalFileSize)
				{
					memset(readBuffer, '\0', sizeof(readBuffer));
					charsRead = read(file_FD, readBuffer, sizeof(readBuffer));
					strcat(dynBuffer, readBuffer);
					totalRead += charsRead;
				}
				FOUND_FILE_FLAG = 1;
			}
			else // FILE NOT FOUND
			{
				dynBuffer = malloc( 16 * sizeof(char));
				memset(dynBuffer, '\0', sizeof(dynBuffer));
				assert(dynBuffer != NULL);
				strncat(dynBuffer, NO_FILE_ERR, 15);
			}

			// Get the data port number, convert to an integer from a string
			dataPortNumber = atoi(client_args[4]);
		}

		// either read file into dynBuffer or not found, so close local directory '.'
		if (closedir(localDir) < 0) error("SERVER: ERROR closing directory.");

		/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
		//		OPEN DATA CONNECTION Q
		/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/

		int dataSocketFD;
		struct sockaddr_in dataAddress;
		struct hostent* dataHostInfo;
		
		// Set up the Q data connection struct
		memset((char*)&dataAddress, '\0', sizeof(dataAddress)); // Clear out the data address struct
		dataAddress.sin_family = AF_INET; // Create a network-capable socket
		dataAddress.sin_port = htons(dataPortNumber); // Store the data port number

		dataHostInfo = gethostbyname(client_args[0]); // Convert the machine name into a special form of address
		if (dataHostInfo == NULL) { fprintf(stderr, "SERVER: ERROR, no such data host\n"); exit(0); }
		memcpy((char*)&dataAddress.sin_addr.s_addr, (char*)dataHostInfo->h_addr, dataHostInfo->h_length); // Copy in the address

		// Set up the socket
		dataSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
		if (dataSocketFD < 0) error("SERVER: ERROR opening data socket");

		// Connect to server
		if (connect(dataSocketFD, (struct sockaddr*)&dataAddress, sizeof(dataAddress)) < 0) // Connect socket to address
			error("SERVER: ERROR connecting to data channel");

		char alias[10]; memset(alias, '\0', 10);
		getFlipAlias(client_args[0], alias);
		printf("Connection from %s.\n", alias);
		/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
		//		SEND DATA TO CLIENT
		/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/

		totalSent = 0;
		if (strcmp(client_args[2], "LIST") == 0) // IF DIRECTORY STRUCTURE
		{
			printf("List directory requested on port %d.\n", dataPortNumber);
			printf("Sending directory contents to %s:%d.\n\n", alias, dataPortNumber);
			fflush(stdout);

			// Send requested data to the client 
			while (totalSent < strlen(dataBuffer))
			{
				charsSent = send(dataSocketFD, dataBuffer, strlen(dataBuffer), 0); 
				if (charsSent < 0) {error("SERVER: ERROR writing to DATA socket"); }
				totalSent += charsSent;
			}
		}
		else if (strcmp(client_args[2], "GET") == 0) // IF FILE REQUESTED
		{
			// informative prompts for server
			printGetRequestStatus(client_args[3], alias, dataPortNumber, FOUND_FILE_FLAG);

			// Send requested data to the client 
			while (totalSent < strlen(dynBuffer))
			{
				charsSent = send(dataSocketFD, dynBuffer, strlen(dynBuffer), 0); 
				if (charsSent < 0) {error("SERVER: ERROR writing to DATA socket"); }
				totalSent += charsSent;
			}
			// CLEAN UP MEMORY (file transfer & 'File not found.')
			free(dynBuffer);
			dynBuffer = NULL;
		}
		/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
		//		SHUT DOWN
		/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/

		// free heap - client commands sent
		destroyTokens(client_args);

		// CLOSE SOCKETS
		close(dataSocketFD); // DATA socket
		close(establishedConnectionFD); // CONTROL socket
	}
	close(listenSocketFD); // Close the Server's listening socket
	return 0; 
}

/********************************************************************
 *
 * Function Name: tokenize()
 * Description:
 * 		Divides char* line parameter into '\0' separated
 * 		inputs so that they can be entered into execvp()
 * 		later in the shell. Memory allocated in tokenize()
 * 		must be freed by use of destroy_tokens() outside
 * 		of function.
 *
 * Paramters: char* line input which is the command line input
 * 		returned from enterLine() function. It is user
 * 		command line input essentially.
 *
 * Returns: char** named token. It is the '\0' terminated array of
 * 		char* each analagous to a single command that will
 * 		be passed into exec() type system call.
 *
 ********************************************************************/
char**  tokenize(char* line)
{
	int total_input_size = (5 * sizeof(char*));
	
	char** tokens;
	char* a_token;

	/* dyn-alloc-memory */
	tokens = malloc(total_input_size);

	if (tokens == 0)
	{
		fprintf(stderr, "%s\n", "Tokens malloc. Exit(1).");
		exit(1);
	}
	memset(tokens, '\0', total_input_size);

	char* noHidePtr;
	/* Re-entrant Tokenization of string input. ***
	*
	*  Modeled in part from style found on stack overflow:
	*  	https://stackoverflow.com/questions/15961253/c-correct-usage-of-strtok-r
	*
	*  1. a_token = strtok_r(line, "@@", &noHidePtr:
	*  	strtok_r is re-entrant tokenizer function from C.
	*  	" " = the delimiters, space ' '.
	*  2. Continue until you hit NULL
	*  3. MUST replace line with NULL after the original strtok_r invoked.
	*******************************************************************************/

	int i = 0;
	for (a_token = strtok_r(line, "@", &noHidePtr); a_token != NULL; a_token = strtok_r(NULL, "@", &noHidePtr))
	{
		if (a_token == NULL) { break; }
		fflush(stdout);
		tokens[i] = a_token;
		i++;
	}
	return tokens;
}

/*************************************************************
 * FUNCTION: destroyTokens()
 *
 * Parameters: char** named tokenArr, the char** to destroy
 *
 * Description:
 * 	Used to deallocate memory from a char**. Cleanup.
 *
 * Returns: Nothing. tokenArr should now be de-allocated,
 * 	and set to NULL.
 *
 ************************************************************/
void destroyTokens(char** tokenArr)
{
	assert(tokenArr != NULL);
	free(tokenArr);
	tokenArr = NULL;
}

/***************************************************************
 * FUNCTION: findFileInDirectory()
 *
 * Params:
 * 	DIR* dirToSearch. A ptr to a directory req'd. to
 * 	manipulate directories in C.
 *
 *  char* targetFileName. The client requested file
 *
 * Dependencies:
 * 	<dirent.h>
 *
 * Description:
 * 	Takes input DIR* dirToSearch is input param to readdir()
 * 	function, which assigns to dirent* struct fileInDirectory.
 * 	Then uses strstr() to search returned dirent* struct to
 * 	see if a file name in the dirent has a substring that
 * 	matches 'targetFileName' parameter. If such a directory
 * 	with that prefix is found, then a pointer to that file
 *  will be returned.
 *
 * Returns:
 * 	char* fileInSub, the file sought.
 *
 * *************************************************************/
char* findFileInDirectory(DIR* dirToSearch, char* targetFileName)
{
	char* fileInSub = NULL;
	
	struct dirent* fileInDirectory;

	// empty directory == 0
	if (dirToSearch > 0)
	{
		// keep looking while there is a file in this directory
		while ((fileInDirectory = readdir(dirToSearch)) != NULL)
		{
			// search for the targetFileName in current directory
			if (strstr(fileInDirectory->d_name, targetFileName) != NULL)
			{
					// save the actual file's name
					fileInSub = fileInDirectory->d_name;
					break;
			}
		}
	}
	return fileInSub;
}

/***************************************************************
 * FUNCTION: getFilesList()
 *
 * Params:
 * 	DIR* dirToSearch. A ptr to a directory req'd. to
 * 	manipulate directories in C.
 *
 *  char* list. buffer to hold file names in dirToSearch
 *              separated by '@' as delimiter character
 *
 * Dependencies:
 * 	<dirent.h>
 *
 * Description:
 * 	Takes input DIR* dirToSearch is input param to readdir()
 * 	function, which assigns to dirent* struct fileInDirectory.
 * 	Then uses strcat() to build up list until NULL is found
 *
 * *************************************************************/
void getFilesList(DIR* dirToSearch, char* list)
{
	// what not to look for
	char* dot = ".";
	char* dotdot = "..";

	char* fileInSub = NULL;
	struct dirent* fileInDirectory;

	// empty directory == 0
	if (dirToSearch > 0)
	{
		// keep looking while there is a file in this directory
		while ((fileInDirectory = readdir(dirToSearch)) != NULL)
		{
			if(strncmp(fileInDirectory->d_name, ".", 1) != 0)
			{
				// concatenate each file in current directory onto list
				strcat(list, fileInDirectory->d_name);
				strcat(list, "\n");
			}
		}
	}

}

/***************************************************************
 * FUNCTION: getFilesSize()
 *
 * Params:
 *		1. int* fd. file descriptor of input file
 *		2. char* file. pointer to file to size
 *
 * Description:
 *		Determines the size in bytes of a file by opening
 *		the file, then calling the file pointer using lseek.
 *		The return of lseek is decremented and passed.
 *
 * Returns:
 *		Size in bytes of specified file. 
 *		File is closed upon return.
 *
 * *************************************************************/
int getFileSize(int* fd, char* file)
{
	// 1. open file
	*fd = open(file, O_RDONLY | S_IRUSR);

	// 2. determine file size
	int size = lseek(*fd, 0, SEEK_END);
	size--;

	// 3. set fd back to beginning
	lseek(*fd, 0, SEEK_SET);
	close(*fd);

	return size;
}

/***************************************************************
 * FUNCTION: getFlipAlias()
 *
 * Params:
 *		1. char* longName. The complete hostname format
 *		2. char* alias. External buffer to store alias
 *
 * Description:
 *		Takes a full flip hostname at OSU (flip1-flip3)
 *		and returns a shorter alias to be used in print
 *		statements. Used when connection to Data Channel
 *		occurs, and for server printing statements.
 *
 * Returns:
 *		The alias of longName.
 *
 * *************************************************************/
void getFlipAlias(char* longName, char* alias)
{
	if (strstr(longName, "flip1") != NULL)
		strcpy(alias, "flip1");
	else if (strstr(longName, "flip2") != NULL)
		strcpy(alias, "flip2");
	else if (strstr(longName, "flip3") != NULL)
		strcpy(alias, "flip3");
	else
		strcpy(alias, "non-flip host");
}

/***************************************************************
 * FUNCTION: printGetRequestStatus()
 *
 * Params:
 *		1. char* file. The file requested by Client
 *		2. char* alias. External buffer containing alias
 *		3. int dataPort. The data port to print
 *		4. int flag. Input FOUND_FILE_FLAG appropriately
 *			(ext. variable, where 1=found)
 *
 * Description:
 *		This prints the appropriate prompt on the server 
 *		depending on the status of the GET request to
 *		transfer file from Server --> Client.
 *
 * *************************************************************/
void printGetRequestStatus(char* file, char* alias, int dataPort, int flag)
{
	if (flag == 1)
	{
		printf("File \"%s\" requested on port %d.\n", file, dataPort);
		printf("Sending \"%s\" to %s:%d.\n\n", file, alias, dataPort);
		fflush(stdout);
	}
	else
	{
		printf("File not found. Sending error message to %s:%d.\n\n", alias, dataPort);
		fflush(stdout);
	}
}


