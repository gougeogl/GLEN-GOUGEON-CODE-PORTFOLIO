* AUTHOR : Glen Gougeon
* CLASS : CS372 Networking
* DUE DATE :	3 - 8 - 2020
* Program: 2 FTP program

ABSTRACT:
	This documentation describes programs ftserver.c and ftclient.py used in conjunction
	as an FTP (File Transfer Protocol) like program. Connection starts on the server on
	a choice of engineering hosts:
		
		flip1.engr.oregonstate.edu (flip1)
		flip2.engr.oregonstate.edu (flip2)
		flip3.engr.oregonstate.edu (flip3)
		
	The longname, or shortname is allowed. Please see below for details on starting these
	programs, but the sequence is as follows:
	
		1. compile server program
		2. chmod +x ftclient.py as needed
		3. start server with a choice of port number. (above 30,000)
			this is the connection the client must use to connect to this server
			
		4. start up client as (after chmod +x):
			
			*commands are -l (list)					
				EXAMPLE: ftclient.py <SERVER_HOST> <SERVER_PORT> -l <DATA_PORT>
			
			* or -g (get) + <FILENAME>
				EXAMPLE: ftclient.py <SERVER_HOST> <SERVER_PORT> -g <FILENAME> <DATA_PORT>
				
					# SAMPLE FILES INCLUDED: Pride_and_Prejudice.txt, big.txt, huge.txt
						* huge.txt is over 11MB so will take a few seconds before arrival
					# these are multiples of the P&P file included.
					
					# FILE SAVED AS 'requestedFile.txt' on CLIENT when transfer complete
			
			* DO NOT USE -l with some <FILENAME>
			
		5. Use up/down arrow key on keyboard to use previous commands as needed for ease
		
/****************************************************************************************
* FileName: ftserver.c
* LANGUAGE : C-99
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
# ***************************************************************************************
# FileName: ftclient.py
# LANGUAGE: Python 3
# LAST-MOD:	3-8-2020 
#
# DESCRIPTION: ftclient : Program 2
#	
#	COMMAND OPTIONS:
#		1. -l (LIST)	Returns a list of files 
#						in the server's current directory
#						(except beginning with '.')
#
#						EXAMPLE: ftclient.py <SERVER_HOST> <SERVER_PORT> -l <DATA_PORT>
#
#		2. -g (GET)		Returns a specific file to the client
#						from server's current directory, or
#						error msg if not available
#
#						EXAMPLE: ftclient.py <SERVER_HOST> <SERVER_PORT> -g <FILENAME> <DATA_PORT>
#
# CREDTIS:
#	Computer Networking: A TOP-DOWN APPROACH
#		by Jim Kurose & Keith Ross, 
#		Pearson Hall, 7th Ed., 2017 
#		TCP client examples Chpt. 2.7 pg 166
#		(used for general design/layout)
#
# HOW TO RUN:
#
#	USE: python3 ftclient.py <SERVER_HOST> <SERVER_PORT> <COMMAND> <FILENAME> <DATA_PORT>
#
#	OR: 1. chmod +x ftclient.py
#		2. ftclient.py <SERVER_HOST> <SERVER_PORT> <COMMAND> <FILENAME> <DATA_PORT>
#
# ****************************************************************************************    

