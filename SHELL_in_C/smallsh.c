/************************************************************************************
* File Name: 	smallsh.c
* Programmer: 	Glen Gougeon
* Class:		CS344 Operating Systems
* Assignment:	Program 3 'smallsh' shell program
* Last Mod.:	7-16-20

* DESCRIPTION:

  		Shell program that mimicks some of the Born-Again Shell (BASH)
		functionality. Accepts most standard Linux commands such as:
		cd, ls, sleep, kill, pwd, echo, etc. including corresponding switches.
		Includes variable expansion of $$ into <pid> of parent process.

		BUILT-INS:
		----------
			'exit' --> To exit shell
			<ENTER> -> To enter a blank line
			   #	-> To enter a comment, first enter hashtag
			 'cd'   -> 'Current Directory' same as Linux cd
		   'status' 
					->	Print the exit status of previously entered command
								-OR-
					->	Print the signal that interrupted 
						the previously entered command

		LOOP-SEQUENCE:
		--------------
			1. Take in raw user input (expecting space-separated commands)
			2. Parse BLANK -OR- comment (#) from raw user input
			3. Expand any instance of $$ into the <pid> of parent process
			4. Tokenize input (space-separated) into an array of char* for later execution
 
 ***********************************************************************************/	
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <errno.h>

/* INPUT PROTOTYPE DECLARATIONS */
char* enterLine();
char* parseComment(char* line);
char* varExpand(char*);
char**  tokenize(char*);
void destroyTokens(char**);

/* BUILT INS PROTOTYPES*/
void BI_CD(char*);
void BI_ST(int);

/* VALIDATION */
int check_bg_selected(char**);

/* RE-DIRECTION */
void redirectFileIO(char**);
int bgRedirect(char** args);

/* COMMAND EXECUTION */
int execute(char**, int);

// BACK-GROUND-CHILD RELATED GLOBALS
int orphan_count = 0;
int orphans[100] = { 0 };
int orphan_flag = 0;

// function to clean, and print completed bg child processes
void childServices(int* orphans, int orphan_count, int orphan_flag);

/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/

int main(int argc, char* argv[])
{
	// control for loop .. becomes 1 if using exit
	int done = 0; 
	int sig_stat_ret_from_execute = 0;

	// INPUT PROTOTYPES
	char* rawInput = NULL;
	char* parsedInput = NULL;
	char* expandedInput = NULL;
	char** result_from_tokenize;

	//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
	//	BUILT-IN COMMANDS  
	char* BI_namesList[5] = {
		"exit",
		"\n",
		"#",
		"cd",
		"status",
	};
	//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
	//	SIGNAL HANDLERS
	//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

	/* related to ignoring cntl-C */	
	struct sigaction ignore_SIGINT = { 0 };
	ignore_SIGINT.sa_handler = SIG_IGN;
	sigfillset(&ignore_SIGINT.sa_mask);
	ignore_SIGINT.sa_flags = 0;

	int fg_only = 0;

	/* related to toggling fore/back ground processes using SIGTSTP */
	void catch_SIGTSTP(int signal)
	{
		if(fg_only == 0)
		{	
			fg_only = 1;
			char* message = "Entering foreground only mode (& is now ignored)\n";
			write(STDOUT_FILENO, message, 49); 
		}
		else if(fg_only == 1)
		{
			fg_only = 0;
			char* message_2 = "Exiting foreground only mode\n";
			write(STDOUT_FILENO, message_2, 29); 
		}
	}

	struct sigaction action_SIGTSTP = { 0 };
	action_SIGTSTP.sa_handler = catch_SIGTSTP; 
	sigfillset(&action_SIGTSTP.sa_mask);
	action_SIGTSTP.sa_flags = SA_RESTART;

	do
	{
		// REGISTER HANDLERS
		sigaction(SIGINT, &ignore_SIGINT, NULL);	// ignores cntl-C
		sigaction(SIGTSTP, &action_SIGTSTP, NULL);	// toggles FORE/BACK-GROUND child processes

		// harvest Zombie-Processes if any
		childServices( orphans, orphan_count, orphan_flag);

		/* GET USER INPUT */
		rawInput = enterLine();	

		/* HANDLE BLANKS AND COMMENTS */
		parsedInput = parseComment(rawInput);

		/* VARIABLE EXPANSION OF $$ into <pid>*/
		expandedInput = varExpand(parsedInput);
		
		/* TOKENIZE INPUT */
		result_from_tokenize = tokenize(expandedInput);	

		/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
		// BLANK	
		if(strcmp(result_from_tokenize[0], BI_namesList[1]) == 0)
		{
			// do nothing
		}
		// COMMENT
		else if((strcmp(result_from_tokenize[0], BI_namesList[2]) == 0)) 
		{
			// do nothing
		}
		// CD
		else if(strcmp(result_from_tokenize[0], BI_namesList[3]) == 0)
		{
			BI_CD(result_from_tokenize[1]);	
		}
		// STATUS
		else if (strcmp(result_from_tokenize[0], BI_namesList[4]) == 0)
		{
			BI_ST(sig_stat_ret_from_execute);
		}
		// EXECUTE
		else if(strcmp(result_from_tokenize[0], BI_namesList[0]) != 0) 
		{
			sig_stat_ret_from_execute = execute(result_from_tokenize, fg_only);
		}	
		// EXIT
		if(strcmp(result_from_tokenize[0], BI_namesList[0]) == 0)
		{ done = 1; }

		/* CLEAN UP MEMORY */
		free(expandedInput);
		expandedInput = NULL;
		destroyTokens(result_from_tokenize);

	} while (done != 1);

	return 0;
}

/*******************************************************************
* Function Name: enterLine()
* Description:
  		Takes user command line input for smallsh.c shell program.
  		replaces terminal newline character with '\0'.
 
* Returns: char* ptr named entry.

 ********************************************************************/
char* enterLine()
{
	char* entry = NULL;
	size_t bufferSize = (2048 * sizeof(char));
	entry = malloc(bufferSize);
	assert(entry != NULL);
	fflush(stdout);

	while (1)
	{
		/* PROMPT  */
		printf(":");

		int check_fail = getline(&entry, &bufferSize, stdin);
		if (check_fail == -1)
		{
			clearerr(stdin);
		}
		else
			break;
	}
	size_t len = strlen(entry);

	if (len == 1 && (strcmp(entry, "\n") == 0))
	{
		/* do not remove newline */
	}
	else if (len > 0 && entry[len - 1] == '\n')
	{
		entry[len - 1] = '\0';
	}

	return entry;
}

/*****************************************************************************
* Function Name: parseComment()
* Description:
  		Takes output from enterLine() and parses out any first #
  		found, and inserts a space between it and what follows.
  		Retains previous structure for style: '# comment' in main.
 
* Parameters:	char* line. The user's raw input
 
* Returns:
  		char* ptr named 'result' if # is first character, and second
  		char is not a space. Modified 'result' will NOT contain #.
		Otherwise returns 'line' to program.

 *****************************************************************************/
char* parseComment(char* line)
{
	if (line[0] == '#' && line[1] != ' ')
	{
		char* result = NULL;

		// Allocate buffer for new output without #
		size_t bufferSize = (2048 * sizeof(char));
		char* buffer = malloc(bufferSize);
		assert(buffer != NULL);
		memset(buffer, '\0', bufferSize);

		// Get input and output lengths
		size_t len_line = strlen(line);
		size_t len_buffer = strlen(buffer);

		strcpy(buffer, line);

		// Shift every char to the right
		// by 1
		int i = 2;
		while (i < len_buffer)
		{
			buffer[i] = line[i - 1];
			i++;
		}

		// over-write # with a space 
		buffer[1] = ' ';

		// make sure null terminated
		buffer[i] = '\0';

		free(line); // free ORIGINAL input
		result = buffer;
		return result;
	}
	else 
	{ 
		return line; 
	}
}

/*****************************************************************************
* Function Name: varExpand()
* Description:
  		Expands any instance of $$ into <pid> of parent process.
 
* Parameters:	char* 'line'. Output from parseComment() function
 
* Returns:
  		char* ptr named 'result' with every $$ instance replaced with
		the current <pid> of parent process.

 *****************************************************************************/
char* varExpand(char* line)
{
	if (line != NULL)
	{
		char* result = NULL;

		// Allocate buffer for new output
		size_t bufferSize = (2048 * sizeof(char));
		char* buffer = malloc(bufferSize);
		assert(buffer != NULL);
		memset(buffer, '\0', bufferSize);

		// Get input and output lengths
		size_t len_line = strlen(line);
		size_t len_buffer = strlen(buffer);

		// Get current Process ID <pid>  
		long thePid = (long)getpid();

		// Allocate buffer for pid
		size_t pidBufferSize = sizeof(long);
		char pidBuffer[pidBufferSize];
		memset(pidBuffer, '\0', pidBufferSize);

		// Write pid as decimal to pidBuffer
		snprintf(pidBuffer, pidBufferSize, "%ld", thePid);
		size_t pidBufferLen = strlen(pidBuffer);

		buffer[0] = line[0];
		int i = 1, j = 1;
		while (i < strlen(line))
		{
			// CHECK IF DOUBLE $$ 
			if (line[i] == '$' && line[i - 1] == '$')
			{
				buffer[j - 1] = '\0';
				buffer[j] = '\0';
				strncat(buffer, pidBuffer, pidBufferLen);
				j += pidBufferLen;
				i++;
			}
			else
			{
				buffer[j] = line[i];
				j++; i++;
			}

		}
		buffer[j + 1] = '\0';

		free(line);
		line = NULL;
		result = buffer;

		return result;
	}
	else 
	{ 
		return line; 
	}
}

/********************************************************************
* Function Name: tokenize()
* Description:
		Breaks up 'line' into an array of char* commands which 
		can be entered into execvp() later in the shell. 
		Memory allocated in tokenize() must be freed by use 
		of destroy_tokens() outside of function.
 
* Paramters: char* line. Output from varExpand() function.
 
* Returns: 
		char** tokens. A NULL '\0' terminated array of
  		char* each analagous to a single command that will
  		be passed into the execvp() system call.

 ********************************************************************/
char**  tokenize(char* line)
{
	/* Allocate memory for tokens array */
	int total_input_size = (512 * sizeof(char*));
	char** tokens;
	char* a_token = NULL;
	tokens = malloc(total_input_size);

	// Assert array not null
	if (tokens == 0)
	{
		fprintf(stderr, "%s\n", "Tokens malloc.");
		exit(1);
	}
	memset(tokens, '\0', total_input_size);

	char* noHidePtr = NULL;
	/* Re-entrant Tokenization of string input. ***********************************
	*
	*  Modeled in part from style found on stack overflow:
	*  	https://stackoverflow.com/questions/15961253/c-correct-usage-of-strtok-r
	*
	*  1. a_token = strtok_r(line, " /", &noHidePtr:
	*  	strtok_r is re-entrant tokenizer function from C.
	*  	" " = the delimiters, space ' '.
	*  2. Continue until you hit NULL
	*  3. MUST replace line with NULL after the original strtok_r invoked.
	*
	*******************************************************************************/

	int i = 0;
	for (a_token = strtok_r(line, " ", &noHidePtr); a_token != NULL; a_token = strtok_r(NULL, " ", &noHidePtr))
	{
		// You hit NULL so exit for-loop
		if (a_token == NULL) { break; }

		fflush(stdout);

		// store tokenized result in array
		tokens[i] = a_token;
		i++;
	}
	return tokens;
}

/************************************************************* 
* Function Name: destroyTokens()
* Description:
  	De-Allocates memory of char** tokenArr.
 
* Parameters: char** named tokenArr, the char** to destroy

* Effect: 
	tokenArr is de-allocated, and set to NULL.

 ************************************************************/
void destroyTokens(char** tokenArr)
{
	assert(tokenArr != NULL);
	free(tokenArr);
	tokenArr = NULL;
}

/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
							BUILT INS
 *MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/

 /*****************************************************************
  * Function Name: BI_CD()
  * Description:
		 Used for directory navigation in smallsh.c shell.
		 "cd" will navigate to home directory, all other
		 combinations of cd [arg] will accecpt both
		 relative and absolute path arguments. Ignores use
		 of "&" used for background process requests in
		 smallsh.c shell

  * Parameters:
		 char* 'whereTo':
			 The directory path to navigate to.
			 (Second argument from output of tokenize().

  * Effect:
		 Shell's current directory is now at requested
		 input 'whereTo'.

  *****************************************************************/
void BI_CD(char* whereTo)
{
	char* myPath = NULL;

	if (whereTo == NULL)
	{
		myPath = getenv("HOME");

		if (chdir(myPath) == -1)
		{
			perror(myPath);
			exit(1);
		}
	}
	else
	{
		errno = 0;
		if (chdir(whereTo) == -1)
		{
			int myerr = errno;
			fprintf(stderr, "Value of errno: %d\n", errno);
			perror(whereTo);
			exit(1);
		}
	}

}

/**********************************************************************
 * Function Name:	BI_ST()
 * Dependencies:	<signal.h>
 * Description:
		Prints either the exit status number of last executed
		command, or the last system signal, but not both.
		Otherwise returns exit value of zero;

 * Parameters:
		int 'input_stat':
			The value returned from execute().

 * Effect:
		Prints Either:
			1. the exit value
			2. signal number interpreted by
				the WEXITSTATUS or WTERMSIG macros.

			*MUTUALLY EXCLUSIVE PRINTING 1 or 2 only.

 **********************************************************************/
void BI_ST(int input_stat)
{
	if (WIFEXITED(input_stat))
	{
		int exitStatus = WEXITSTATUS(input_stat);
		printf("exit value %d\n", exitStatus);
		fflush(stdout);
	}
	else if (WIFSIGNALED(input_stat) != 0)
	{
		int termSignal = WTERMSIG(input_stat);
		printf("terminated by signal %d\n", termSignal);
		fflush(stdout);
	}
	else
	{
		printf("exit value 0\n");
		fflush(stdout);
	}
}

/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
							VALIDATION
 *MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/

/*****************************************************************
 * Function Name: check_bg_selected()
 * Description:
		determines if arguments list originally parsed
		by tokenize() function contains request for a
		background execution as last parameter of command
		list "&".

 * Parameters: char** named arguments, the list of arguments to
		evaluate to see if "&" is at the end end of the
		list. (NULL is technically at the end, but not
		considered here).

 * Returns:
		1. if "&" found at END of list
		0. (default) if anything else
 ****************************************************************/
int check_bg_selected(char** arguments)
{
	int result = 0;

	int i = 0;
	while (arguments[i] != NULL) { i++; }

	if (strcmp(arguments[i - 1], "&") == 0)
	{
		result = 1;
	}

	return result;
}

/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
						I/O RE-DIRECTION
 *MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/

/********************************************************************************
* Function Name: redirect()
* Description:
  		Handles redirection required for opening and/or creating file requests.
		Output file request for non-existing files will be created and opened.
  
* Parameters: 
		char** named args. the arguments to evaluate for presence of: 
  			'<': Input File
			'>': Output File
			* OR COMBINATION OF BOTH

 
* Effect: 	
		1. successful redirection of input, output, or both to the
 		appropriate file path.
 
  		2. args ready to be passed directly into execvp(). 
 
 ******************************************************************************/
void redirectFileIO(char** args)
{
	// FILE DESCRIPTORS & check for successful dup2  
	int fd_three, fd_four, check_fd;

	int idx;
	for(idx = 0; args[idx] != NULL; idx++)
	{
		// CHECK IF INPUT FILE 
		if((strcmp(args[idx], "<") == 0) && args[idx + 1] != NULL)
		{
			// open a file for reading 	
			fd_three = open(args[idx + 1], O_RDONLY);
			if(fd_three < 0)
			{
				fprintf(stderr, "Cannot open %s for input\n", args[idx+1]);
				fflush(stdout);
				exit(1);
			}

			// evaluate if redirection succeeded  
			check_fd = dup2(fd_three, STDIN_FILENO);
			if(check_fd < 0) 
			{ 
				perror("dup2 STDIN"); 
				exit(1); 
			}
			args[idx] = NULL;	// <---------- This overwrite ensures execvp() will work 
								//	execvp() will stop at NULL, and that is what
								//	you want. It is analagous to placing a NULL
								//	at the end of all cmd arguments 
		}
		// CHECK IF OUTPUT FILE 
		else if((strcmp(args[idx], ">") == 0) && args[idx + 1] != NULL)
		{
			// create and open a file for writing  
			fd_four = open(args[idx + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if(fd_four < 0)
			{
				fprintf(stderr, "Could not create %s for output\n", args[idx+1]);
				fflush(stdout);
				exit(1);
			}

			// evaluate if redirection succeeded  
			check_fd = dup2(fd_four, STDOUT_FILENO);
			if(check_fd < 0) 
			{ 
				perror("dup2 STDOUT"); 
				exit(1); 
			}
			args[idx] = NULL;
		}
	
	} // END LOOP 
		
}

/******************************************************
* Function: bgRedirect()
* Params:	char** args. The argument list for shell
* Description:
		Redirects standard output to /dev/null if '&' is
		found as last argument of 'args' parameter.

* Returns:
		0: if last command was NOT '&'
		1: if '&' WAS the last command

*******************************************************/
int bgRedirect(char** args)
{
	int fd = 0;
	int checkFD = 0;
	int i = 0;

	// cycle to last index
	while (args[i] != NULL) { i++; }

	if (strcmp(args[i - 1], "&") == 0)
	{
		fd = open("/dev/null", O_WRONLY);
		if (fd < 0)
		{
			perror("BG problem, cannot open: /dev/null");
			exit(1);
		}

		// RE-DIRECT BG OUTPUT 	
		checkFD = dup2(STDOUT_FILENO, fd);
		if (checkFD < 0)
		{
			perror("dup2 BG STDOUT");
			fflush(stdout);
			exit(1);
		}

		args[i - 1] = NULL;
	}
	return checkFD;
}

/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
							COMMAND EXECUTION
 *MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/

/* Executes non Built In commands. Needs redirect() and 
 * destroyTokens() functions. cmd input left unharmed.  */

/***************************************************************************
 * Function Name: execute()
 * Dependencies: 
		GLOBALS
		-------
			int orphan_count = 0;
			int orphans[100] = { 0 };
			int orphan_flag = 0;

		HELPERS
		-------
			check_bg_selected() --> VALIDATION
			redirectFileIO()	--> FILE I/O
			bgRedirect()		--> STDOUT redirection
			childServices()		--> child <pid> cleanup

 * Description:
  		Executes non-built-in commands to shellsh.c shell.
 
  		Calls check_bg_selected to determine if background
  		process request will occur. Function will spawn off
  		a child as a foreground or background child process,
  		depending on result from check_bg_selected. 1 if true,
  		zero otherwise.
 
  		The parent process (shell) will continue to run.
  		CNTL-C (SIGINT) is ignored in the parent. Foreground
  		child processes are terminated by CNTL-C (SIGTERM).
  		
  		A CNTL-Z to Parent will print a message, and requests
  		to start a background process will be ignored.
  		Child processes (after fork) will ignore CNTL-Z (SIGTSTP)
  		signals. 
 
 * Paramters: 
		char** cmd. Array of char* commands to be executed in execvp()
		int fg_only_request. Flag set by possible cntl-Z in parent
 
 * Effect: Child process requests executed. 

 * Returns:
		A int that indicates either the result of a status, or
  		of an interruption caused by a signal. The int must be
  		interpreted by BI_ST(), or use of macros WEXITSTATUS, or
  		WTERMSIG as appropriate. 

 **************************************************************************/
int execute(char** cmd, int fg_only_request)
{
	int childStatus = -5;
	pid_t child_pid = -5;

	// FLAG TO DETERMINE WHERE TO SEND I/O 
	int bg_selected = check_bg_selected(cmd);

	// CALL FORK FOR NEW PROCESS 	
	child_pid = fork();

	// SIGNAL HANDLER: ignore cntl-Z (SIGTSTP) a.k.a. Terminal Stop
	struct sigaction ignore_SIGTSTP = { 0 };
	ignore_SIGTSTP.sa_handler = SIG_IGN;
	sigemptyset(&ignore_SIGTSTP.sa_mask);
	sigaddset(&ignore_SIGTSTP.sa_mask, SIGTSTP);
	ignore_SIGTSTP.sa_flags = 0;

	// SIGNAL HANDLER: allow fg child cntl-C 	
	struct sigaction allow_SIGINT = { 0 };
	allow_SIGINT.sa_handler = SIG_DFL;
	sigemptyset(&allow_SIGINT.sa_mask);
	sigaddset(&allow_SIGINT.sa_mask, SIGINT);
	allow_SIGINT.sa_flags = 0;

	switch(child_pid)
	{
		// ERROR
		case -1:	{perror("bad fork."); break;}
		
		// CHILD
		case 0:
		{
			// background operation MUST have these conditions
			if((bg_selected == 1) && (fg_only_request == 0)) 
			{
				printf("background pid is %d\n:", getpid() );
				fflush(stdout);

				bgRedirect(cmd); // <-------- redirects stdout		

			}
			// background selected but is foreground ONLY
			else if((bg_selected == 1) && (fg_only_request == 1))
			{
				int idx = 0;
				while(cmd[idx] != NULL) 
				{ 
					idx++; 
				}

				// remove '&' to IGNORE background requests
				if(strcmp(cmd[idx-1], "&") == 0){ cmd[idx-1] = NULL; }

				// REGISTER: normal cntl-C
				sigaction(SIGINT, &allow_SIGINT, NULL);
			}
			else
			{
				/* Accept normal cntl-C in foreground child */
				redirectFileIO(cmd); // <------ new read/write files 	

				// REGISTER: normal cntl-C
				sigaction(SIGINT, &allow_SIGINT, NULL);
			}

			// now execute
			if (execvp(cmd[0], cmd) == -1)
			{
				perror(cmd[0]);
				exit(1);
			}
			break;
		}
		// PARENT
		default:
		{
			if((bg_selected == 1) && (fg_only_request == 0))
			{
				// save pid for cleanup later by childServices()
				orphans[orphan_count] = child_pid;
				orphan_count++;
					
				// SET THIS B/C YOU HAVE ORPHAN PROCESSES IN ARRAY
				orphan_flag = 1;

				// If child isn't done..continue anyway
				child_pid = waitpid(child_pid, &childStatus, WNOHANG);

			}	
			else
			{
				// Wait/Block until Child done
				child_pid = waitpid(child_pid, &childStatus, 0);	
				
				// If cntl-C terminated foreground child, print immediately
				if(WIFSIGNALED(childStatus) != 0)
				{
					int termSignal = WTERMSIG(childStatus);
					printf("terminated by signal %d\n", childStatus);
					fflush(stdout);
				}
				
			}
			break;

		} // END DEFAULT

	} // END SWITCH
	return childStatus;
}

/******************************************************************
 * Function Name: childServices() 
 * Description:
 		Designed to clean up any remaining background child
 		processes for the smallsh.c shell program.
 
 * Parameters: 
  		1. int* orphans: 
			The array of background child pids 
			to be waited on for cleanup.
 
  		2. int orphan_count: 
			index for the orphans array
 
  		3. int orphan_flag: 
			global in main function. Set if a child
			zombie <pid> needs to be harvested
  		
 * Returns: 
  		1. ALL children in 'orphans' array cleaned up.
  		2. print of who was cleaned up, including their pid
  		2b. status of who was cleaned up, by exit status or signal
  		3. orphan_flag is reset.

 ******************************************************************/
void childServices(int* orphans, int orphan_count, int orphan_flag)
{
	// if orphans exist
	if(orphan_flag)
	{
		int childStatus = -5;
		while(	(orphans[orphan_count] = waitpid(-1, &childStatus, WNOHANG)) > 0 )
		{
			printf("background pid %d is done. ", orphans[orphan_count]);
			fflush(stdout);
			BI_ST(childStatus);
			fflush(stdout);
			orphan_count--;
		}				
		// reset orphan flag
		orphan_flag = 0;
	}
}

/******************************* EOF **********************************/
