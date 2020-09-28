#!/usr/bin/env python3

# *****************************************************************************
# FileName: clean.py
# Author:   Glen Gougeon
# Date:     9-25-20
# Last Mod: 9-25-20
#
# Description:
#   Simplifies removing executables, and files generated from running
#   the ftserver.c and ftclient.py programs
#
#   1. delete ftserver executable
#   2. delete pickPort executable
#   3. delete BOTH port files created
#   4. delete requestedFile.txt which is where the result of -g [FILE]
#       was stored by ftclient.py
#
# *****************************************************************************
import os

server = "rm ftserver"
picker = "rm pickPort"
ports = "rm SERVER_PORT DATA_PORT"
files = "rm requestedFile.txt"
os.system(server)
os.system(picker)
os.system(ports)
os.system(files)
