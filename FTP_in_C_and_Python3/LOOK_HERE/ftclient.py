#!/usr/bin/env python3

# ***************************************************************************************
# FileName: ftclient.py
# LANGUAGE: Python 3
# AUTHOR:	Glen Gougeon
# CLASS:	CS372 Networking 
# DUE DATE:	3-8-2020
# LAST-MOD:	9-25-2020 
#
# DESCRIPTION: ftclient : Program 2
#	
#	COMMAND OPTIONS:
#		1. -l (LIST)	Returns a list of files 
#				in the server's current directory
#				(except beginning with '.')
#
#				EXAMPLE: ftclient.py <SERVER_HOST> <SERVER_PORT> -l <DATA_PORT>
#
#		2. -g (GET)	Returns a specific file to the client
#				from server's current directory, or
#				error msg if not available
#
#				EXAMPLE: ftclient.py <SERVER_HOST> <SERVER_PORT> -g <FILENAME> <DATA_PORT>
# CREDITS:
#		Computer Networking: A TOP-DOWN APPROACH
#		by Jim Kurose & Keith Ross, 
#		Pearson Hall, 7th Ed., 2017 
#		TCP client examples Chpt. 2.7 pg 166
#			(used for general design/layout)
#
# HOW TO RUN:
#
#	USE: python3 ftclient.py <SERVER_HOST> <SERVER_PORT> <COMMAND> <FILENAME> <DATA_PORT>
#
#	OR: 1. chmod +x ftclient.py
#	    2. ftclient.py <SERVER_HOST> <SERVER_PORT> <COMMAND> <FILENAME> <DATA_PORT>
#
# ****************************************************************************************    

import sys 
import re
from socket import* 

def validate_file_name(fn):
    if (re.search(r'[`~!^@#$%&*(){[\]}=+\\?,<>]', fn)): 
        return False 
    else:
        return True 

def validate_port(pn):
    if (re.search(r'[0-9]', pn)):
        return True
    else:
        return False

# VALIDATE ARGUMENT COUNT
if (len(sys.argv) < 5):
    sys.stdout.write("ERROR: Argument Count\n")
    sys.exit()    

host = ""
port = ""

# VALIDATE HOST NAME (which flip)
def validate_host(hn):

    if 'flip1' in hn:
        return 'flip1.engr.oregonstate.edu'

    elif 'flip2' in hn:
        return 'flip2.engr.oregonstate.edu'

    elif 'flip3' in hn:
        return 'flip3.engr.oregonstate.edu'

    elif 'localhost' in hn:
        return 'localhost'

    else:
        sys.stdout.write("CLIENT: ERROR Invalid Server\n") # <-- error msg here
        sys.exit()

serverHost = validate_host(sys.argv[1])

def get_nick_name(nn):
    if 'flip1.engr.oregonstate.edu' in nn:
        return 'flip1'

    elif 'flip2.engr.oregonstate.edu' in nn:
        return 'flip2'

    elif 'flip3.engr.oregonstate.edu' in nn:
        return 'flip'

    elif 'localhost' in nn:
        return 'localhost'

    else:
        return 'not flip server'

host = get_nick_name(serverHost)

# VALIDATE SERVER CONTROL PORT
portStat = validate_port(sys.argv[2])
if (portStat): 
    serverPort = sys.argv[2]
else:
    sys.stdout.write("CLIENT: ERROR invalid host port\n") # <-- error msg here
    sys.stdout.flush()
    sys.exit()

# VALIDATE <COMMAND>
dataStat = ""
cmd = sys.argv[3]
if '-g' in cmd:
    # VALIDATE <FILENAME>
    fileStat = validate_file_name(sys.argv[4]) 
    if (fileStat):
        requestedFile = sys.argv[4]

        # VALIDATE <DATA_PORT>
        dataStat = validate_port(sys.argv[5])
        if (dataStat):
            dataPort = sys.argv[5] # the port to open DATA CHANNEL on
        else:
            sys.stdout.write("CLIENT: ERROR invalid data port format.\n") # <-- error msg here
            sys.exit()

    else:
        sys.stdout.write("CLIENT: ERROR invalid file name format.\n") # <-- error msg here
        sys.exit()

elif '-l' in cmd:
    # VALIDATE <DATA_PORT>
    dataStat = validate_port(sys.argv[4])
    if (dataStat):
        dataPort = sys.argv[4] # alternate <DATA_PORT> if LIST is cmd

else:
    sys.stdout.write("CLIENT: ERROR invalid command requested.\n") # <-- error msg here
    sys.exit()

# SET UP CONTROL CONNECTION (P) & CONNECT
controlPort = int(serverPort) # server control port
controlSocket = socket(AF_INET, SOCK_STREAM) # socket
controlSocket.connect((serverHost, controlPort)) # connect to socket

#MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
#		APPEND DATA FOR SERVER onto 'data'
#MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

# APPEND which flip CLIENT is on
thisHost = getfqdn()

# APPEND <SERVER_HOST>
data =  thisHost + "@" + serverPort + "@"

# APPEND <COMMAND>
if '-l' in cmd:
    data += "LIST@"
elif '-g' in cmd:
    data += "GET@"
	# APPEND <FILENAME>
    data += requestedFile + "@"

# APPEND <DATA_PORT>
data += dataPort + "@"

# send all cmd line params (bundled 'data') to server for processing
controlSocket.send(data.encode())

# SETUP DATA CONNECTION (Q) & WAIT
dataRequestSocket = socket(AF_INET, SOCK_STREAM)
dataRequestSocket.bind((thisHost, int(dataPort)))
dataRequestSocket.listen(1)

# ACCEPT DATA CONNECTION (from server)
dataRecvSocket, addr = dataRequestSocket.accept()

# MUST setup array for receiving data
# needed b/c cannot .decode() partial data
# or recv() will return err
arr = bytearray()
arr.clear()
incomingData = ""
flag = True

while flag is True: 
	# receive requested data from SERVER
    incomingData = dataRecvSocket.recv(1024)

    # pure byte data appened into array
    arr.extend(incomingData)

    if (len(incomingData) <= 0):
        flag = False 
        break

    incomingData = ""

# convert from bytes to readable string
allData = arr.decode(encoding='utf8')

# GET FILE was requested
if '-g' in cmd:
    if ('File not found.' not in allData): # IF FILE EXISTS
        sys.stdout.write("\nReceiving \"%s\" from " % requestedFile)
        sys.stdout.write("%s:" % host)
        sys.stdout.write("%s\n\n" % dataPort)

        # create new file
        f1 = open("requestedFile.txt", "w+")
        f1.write(allData)

        # close file
        f1.close()
        print('File transfer complete.')
        sys.stdout.write("NOTE* SEE: requestedFile.txt for contents.\n\n")

    else: # FILE NOT FOUND
        sys.stdout.write("\n%s:" % host)
        sys.stdout.write("%s says\n" % dataPort)

	# print error msg
        sys.stdout.write("%s\n\n" % allData)
        sys.stdout.flush()

# LIST DIRECTORY was requested
else:
    sys.stdout.write("\nReceiving directory structure from %s:" % host) 
    sys.stdout.write("%s\n\n" % dataPort)
	# print directory structure
    sys.stdout.write("%s\n\n" % allData)
    sys.stdout.flush()

# CLOSE SOCKETS
dataRequestSocket.close()
controlSocket.close()
# EOF
