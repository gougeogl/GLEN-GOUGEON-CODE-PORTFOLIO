/****************************************************************
* FileName:	fileHelpers.c
* Author:	Glen Gougeon
* Created:	12-7-2019
* Last Mod:	7-28-20
* Class:	CS344 Operating Systems I

* Description:	OTP Client-Server Project

***************************************************************/
#include "helpers.h"

/*****************************************************************
* Function: isValidValue()
* Description:
	Validation Helper.
	Checks if a buffer/string contains anything other than the
	26 capital English letters and/or SPACE.

* Returns:
	if error: -1
	if bad characters found: 0 (invalid)
	if every character is [A-Z, SPACE]: 1 (isValid)

******************************************************************/
int isValidValue(char* str, int length)
{
	int result = -1;

	for (int i = 0; i < length; i++)
	{
		int value = (int)str[i];

		if (value < 32 || value > 90) // ascii 'space' = 32, ascii 'Z' = 90
		{
			if (value == 10) // ascii newline LF
			{
				break;
			}
		}
		else if (value >= 65 && value <= 90) // ascii 'A' = 65
		{
			result = 1;
		}
		else if (value == 32)
		{
			result = 1;
		}
		else
		{
			result = 0;
			break;
		}
	}
	return result;
}

/************************************************************
* Function: _initBuffer()
* Description:
	Helper function to dynamically allocate a buffer
	according to [size].

* Returns:
	Buffer initialized with NULLs as output, -OR-
	an error message to stderr.

*************************************************************/
char* _initBuffer(int size)
{
	int bufferSize = size * sizeof(char);
	char* buffer = malloc(bufferSize);
	memset(buffer, '\0', bufferSize);
	if (buffer == NULL) error("_initBuffer is NULL!");
	return buffer;
}

/**************************************************************
* Function: getFileSize()
* Returns:
	The size in bytes of [file] as an integer on success -OR-
	-1 (if error)

***************************************************************/
int getFileSize(char* file)
{
	int size = 0;

	int fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror(file);
		fflush(stdout);
		size = -1;
	}
	else
	{
		// GET FILE SIZE IN BYTES
		off_t fileBytes = 0;
		fileBytes = lseek(fd, 0, SEEK_SET);
		fileBytes = lseek(fd, 0, SEEK_END);
		close(fd);

		size = (int)fileBytes;	
	}
	return size;
}

/**************************************************************
* Function: readFileLoop()
* Parameters:
	1. char* file. The file to open and read.
	2. char* buffer. A buffer allocated outside of this
	   function is called. It will store all of the file.
	3. int fileSize. The size of [file] input.

* Description:
	Helper function to read entire contents of [file]
	into [buffer]. Null terminated.

* Effect:
	Network interruptions and other blocking system calls
	can cause read to fail. This function serves as a 
	wrapper to ensure all bytes are read into buffer.

***************************************************************/
void readFileLoop(char* file, char* buffer, int fileSize)
{
	int fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror(file);
		fflush(stdout);
		exit(1);
	}
	else
	{
		// setup temp buffer for looping sequence
		char tempBuffer[256];
		memset(tempBuffer, '\0', sizeof(tempBuffer));

		// ensure file starts at beginning
		lseek(fd, 0, SEEK_SET);

		int totalRead = 0;
		int bytesRead = 0;

		while (totalRead < fileSize)
		{
			bytesRead = read(fd, tempBuffer, sizeof(tempBuffer));
			strncat(buffer, tempBuffer, bytesRead);
			memset(tempBuffer, '\0', sizeof(tempBuffer));
			totalRead += bytesRead;
			bytesRead = 0;
		}
		close(fd);

		buffer[strcspn(buffer, "\n")] = '\0';
	}
}

/**************************************************************
* Function: writeFileToBuffer()
* Parameters:
	1. char* file. The file to use.
	2. int* fileBytes. The size/length of the file to use.

* Description:
	Converts a file into a dynamically allocated buffer
	for use over the network. Makes calls to helpers:
		getFileSize()
		_initBuffer()
		readFileLoop()

* Returns
	Contents of [file] as output to be assigned to a char*
		            
***************************************************************/
char* writeFileToBuffer(char* file, int* fileBytes)
{
	// GET SIZE
	*fileBytes = getFileSize(file);

	// CREATE MAIN BUFFER
	char* buffer = _initBuffer(*fileBytes);

	// READ ENTIRE FILE
	readFileLoop(file, buffer, *fileBytes);

	return buffer;
}
