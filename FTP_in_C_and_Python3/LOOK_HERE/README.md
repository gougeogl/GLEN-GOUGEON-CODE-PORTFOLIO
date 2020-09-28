# FTP CLIENT-SERVER in C and Python3 DOCUMENTATION
* AUTHOR :	Glen Gougeon
* CLASS :	CS372 Networking
* DATE :	9-25-20
* Last Mod:	9-28-20

#### Programs:
* ftserver.c
    * LANGUAGE : C-99

* ftclient.py
    * LANGUAGE: Python 3

## DESCRIPTION:
An implementation of a File Transfer Protocol (FTP) system.
This was an assignment for an Introductory Networking class,
which was designed to help us learn how FTPs work, and to give
practise in mixing languages.

These programs were **not** designed with *any* security features.
**DO NOT** use except on *localhost*. 

##### Note: Network Access restricted *modified from original*
Oregon State University has 3 engineering servers which I had access to.
Since you will likely not have access without their permission, I have
modified the program to run on **localhost** to simulate functionality.

## QUICK START GUIDE 
* Starting the Server:
1. Open a terminal in directory where you cloned the repo.
1. Options:
    1. make run
    1. ./start.py
1. Run the Client:
    1. Open a SECOND terminal in directory where you cloned the repo.

* Note
> The 2nd terminal is so you can see what the server and client
> and server are doing. Each program shows different output.

### Quick Start Commands
* To _LIST_ the files on the server _(after server started. SEE above)_ 
    * **./request -l**

* To _GET_ a _file_ from the server 
    * **./request -g {FILE}**


#### HELPER SCRIPTS *to make your life easier* include:
* pickPort.c
    * Generates a port number {55k..56k} 

* start.py
    * Compiles pickPort.c ftserver.c
    * Stores port numbers to files: *SERVER_PORT* & *DATA_PORT*
    * Starts ftserver on a port 
    * TO STOP: Control-C

* request.py
    * Simplifies normal command line input to ftclient.py 

* clean.py
    * Removes executables
    * destroys files: *SERVER_PORT* & *DATA_PORT*

#### Sample Files:
I have included some sample files to try transferring if desired.
* 'Pride_and_Prejudice.txt'. The entire book.
    * This was our *large* file.. but I included a larger one here.
* big.txt. P&P copied a few times into a larger file.	

## PROGRAMS DETAILS:
### ftserver.c
**Description:**
Server program for File Transfer Protocol - like program.
Used in conjunction with ftclient.py to transfer files to client.

**HOW TO RUN:**
1. COMPILE: gcc -o ftserver ftserver.c
2. RUN:	 ./ftserver **PORT_NUM** 
* PORT_NUM: 
> is the port to bind to

**TO QUIT:** Control-C

### ftclient.py:
**HOW TO RUN:**
* if ftclient.py not executable (green)
> chmod +x ftclient.py

*Commands* | LIST | GET
-----------|------|----
enter | -l | -g
action | returns a list of files on the server | used to retrieve a file from the server			

**SYNTAX**
* LIST: 
ftclient.py <SERVER_HOST> <SERVER_PORT> -l <DATA_PORT>

* GET:
ftclient.py <SERVER_HOST> <SERVER_PORT> -g <FILENAME> <DATA_PORT>

### CREDITS:
Prof. Benjamin Brewster's Lectures, Slides, and sample code used
as base-point for simple client-server connections in C. The error
function is his, and some of the formatting of the send, recv calls
are very similar.

* Computer Networking: A TOP-DOWN APPROACH
> by Jim Kurose & Keith Ross, 
> Pearson Hall, 7th Ed., 2017 
> TCP client examples Chpt. 2.7 pg 166
> (used for general design/layout)

* *Note*
> I have re-used code I designed for different projects in CS344,
> namely findFileInDirectory was updated and simplified portion  
> of the build-rooms game, and tokenize was part of the smallsh.c
> program. See function descriptions for more details            

