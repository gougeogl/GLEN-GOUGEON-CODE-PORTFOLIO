/*********************************************************
 * FileName: pickPort.c
 * Author:	Glen Gougeon
 * Created:	9-24-20
 * Last Mod:	9-24-20
 
 * Description:
 	Picks a port number for ftserver.c program
	between MIN and MAX by call to generatePort().
    	Creates 2 files: SERVER_PORT & DATA_PORT to
	hold the port numbers for later access.	

 *********************************************************/
#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

#define MIN 55000
#define MAX 56000
#define MAX_BUFFER 6

unsigned int generatePort();

int main()
{
	unsigned int seed;
	time_t sinceEpoch;
	seed = time(&sinceEpoch);
	srand(seed);

	size_t bufferSize = MAX_BUFFER * sizeof(char);
	char buffer[bufferSize];
	memset(buffer, '\0', bufferSize);

	/*WWWWWWWWWWWWWW
	   SERVER PORT 
	MMMMMMMMMMMMMMM*/
	unsigned int num = generatePort();

	snprintf(buffer, bufferSize, "%u", num); 

	// open and write to input SERVER param file
	int fd = open("SERVER_PORT", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	write(fd, buffer, bufferSize);
	close(fd);

	/*WWWWWWWWWWWW
	   DATA PORT 
	MMMMMMMMMMMMM*/
	
	memset(buffer, '\0', bufferSize);

	// get a port
	num = generatePort();

	snprintf(buffer, bufferSize, "%u", num); 

	// open and write to input SERVER param file
	fd = open("DATA_PORT", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	write(fd, buffer, bufferSize);
	close(fd);
	
	return 0;
}

/*******************************************************
 * Function: generatePort()
 * Description:
 	generates an unsigned int in the range:
	[MIN..MAX]
	num = rand() % [MAX-MIN + 1] + MIN 

********************************************************/
unsigned int generatePort()
{
	return rand() % (MAX - MIN + 1) + MIN; 
}
