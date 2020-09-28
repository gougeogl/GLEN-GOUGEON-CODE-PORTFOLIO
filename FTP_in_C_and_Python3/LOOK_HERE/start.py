#!/usr/bin/env python3

# *****************************************************************************
# FileName: start.py
# Author:   Glen Gougeon
# Date:     9-25-20
# Last Mod: 9-25-20
#
# Description:
#   Simplifies starting ftserver.c program and selection of required port.
#
#   1. Compiles C programs:
#       ftserver.c pickPort.c
#   2. Executes pickPort to generate random ports for server connection
#       and data connection.
#   3. Reads contents of SERVER_PORT file into local
#   4. Reads contents of DATA_PORT file into local
#   5. Starts ftserver.c program with SERVER_PORT value as input
#
# *****************************************************************************
import os
import sys

# Compile ftserver.c program 
os.system("gcc -o ftserver ftserver.c")

# Compile pickPort.c
os.system("gcc -o pickPort pickPort.c")

# Generate ports 
# files: SERVER_PORT & DATA_PORT
os.system("./pickPort")

# Open SERVER_PORT file
f1 = open("SERVER_PORT", "r")

# Read into variable
portToUse = f1.read(5) # read 5 bytes (chars)

# Close SERVER_PORT file
f1.close()

# Declare program call
serverProgram = "./ftserver "

# build up string
command = serverProgram + portToUse

# print out DATA_PORT as suggestion
f2 = open("DATA_PORT", "r")
dataPort = f2.read(5)
sys.stdout.write("Generated Data Port: %s\n" % dataPort)
sys.stdout.write("TO EXIT PROGRAM: cntl-C\n")

# Start server [PORT#]
os.system(command)
