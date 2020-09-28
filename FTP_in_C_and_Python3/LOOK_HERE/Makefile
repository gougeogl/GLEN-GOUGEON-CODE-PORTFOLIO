# FileName: Makefile
# Program: ftserver.c
# Date: 9-24-20

# Description:
# 	Compiles the File Transfer Protocol (FTP) server for
# 	ftserver.c program. Cleans all created files at the end.

pickPort: pickPort.c
	gcc -o pickPort pickPort.c

ftserver: ftserver.c
	gcc -o ftserver ftserver.c

run:
	./start.py
clean:
	./clean.py
