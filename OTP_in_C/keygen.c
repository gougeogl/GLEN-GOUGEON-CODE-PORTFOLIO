/*******************************************************
 * FileName: 	keygen.c
 * Author:	Glen Gougeon
 * Class:	CS344 Operating Systems I
 * Created:	11-29-2019
 * Last Mod: 	12-4-2019
 * Assignment:	Program 4
 *
 * Description:
 * 	This program creates a key file of specified length. 
 * 	The characters in the file generated will be any of the 27 allowed characters, 
 * 	generated using the standard UNIX randomization methods. 
 * 	Do not create spaces every five characters, as has been historically done. 
 * 	Note that you specifically do not have to do any fancy random number generation: 
 * 	we’re not looking for cryptographically secure random number generation! rand() is just fine. 
 * 	The last character keygen outputs should be a newline. 
 * 	All error text must be output to stderr, if any.
 *
 * ***************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_SIZE 70000

int main( int argc, char* argv[] )
{
	time_t t;
	unsigned int seed = 0;
	seed = time(&t);
	srand(seed);

	// create a buffer for the actual key
	size_t MAX_BUFFER_SIZE = ( MAX_SIZE * sizeof(char));
	char* buffer = malloc( MAX_BUFFER_SIZE );
	memset(buffer, '\0', MAX_BUFFER_SIZE);

	int idx = atoi(argv[1]);

	// 65-90 = ascii upper case, 32 = ascii space
	unsigned int MAX = 90;
	unsigned int MIN = 32;

	// return from call to rand()
	unsigned int value = 0;

	int tally = 0;
	while(idx != tally)
	{
		value = rand() % (MAX - MIN + 1) + MIN;
		if(value >= 65 || value == 32)
		{
			buffer[tally] = (char)value;
			tally++;
		}
	}

	printf("%s\n", buffer);	

	// memory for buffer 
	free(buffer);
	return 0;
}
/* EOF */
