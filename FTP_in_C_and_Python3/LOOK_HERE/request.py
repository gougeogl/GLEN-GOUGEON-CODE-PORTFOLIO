#!/usr/bin/env python3

# *****************************************************************************
# FileName: request.py
# Author:   Glen Gougeon
# Date:     9-25-20
# Last Mod: 9-25-20
#
# Description:
#   Simplifies call to ftclient.py script following the use of start.py.
#   Arguments to ftclient.py either: 
#
#       [-l] LIST FILES on server: 
#           ftclient.py <SERVER_HOST> <SERVER_PORT> -l <DATA_PORT>
#
#       [-g] GET a file from server: 
#           ftclient.py <SERVER_HOST> <SERVER_PORT> -g <FILENAME> <DATA_PORT>
#
#       SIMPLIFIED BY THIS SCRIPT:
#
#       ./request -l:   To list files on server
#       ./request -g [FILE] to get a file from server
#
# *****************************************************************************

import os
import sys

sys.stdout.write("Running Script..   : '%s'\n" % sys.argv[0])
sys.stdout.write("Network Connection : localhost ONLY\n")
sys.stdout.write("**********************************************************************************\n")
if '-l' in sys.argv[1]:
    sys.stdout.write("built LIST-command : ftclient.py <SERVER_HOST> <SERVER_PORT> -l <DATA_PORT>\n")
elif '-g' in sys.argv[1]:
    sys.stdout.write("built  GET-command : ftclient.py <SERVER_HOST> <SERVER_PORT> -g <FILE> <DATA_PORT>\n")

sys.stdout.write("**********************************************************************************\n")

# access the SERVER_PORT num
f1 = open("SERVER_PORT", "r")
serverPort = f1.read(5)
f1.close()

# access the DATA_PORT num
f2 = open("DATA_PORT", "r")
dataPort = f2.read(5)
f2.close()

# accept from input.. either -l or -g to build up a request
def appendCommand(option):
    appendage = ""
    if '-l' in option: 
        appendage = '-l ' + dataPort

    elif '-g' in option: 
        if len(sys.argv) != 3:
            print('Missing: file name for GET command')
            sys.exit()
        else:
            appendage = '-g ' + sys.argv[2] + ' ' + dataPort

    return appendage

# use localhost for serverHostName in this script
command = './ftclient.py localhost ' + serverPort + ' '      
command += appendCommand(sys.argv[1])

# Execute built-up command entry
os.system(command)
