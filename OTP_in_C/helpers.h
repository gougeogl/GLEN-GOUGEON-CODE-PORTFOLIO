/****************************************************************
* FileName: helpers.h
* Author:	Glen Gougeon
* Date:		7-28-20
* Last MOD:	7-28-20
* Class:	CS344 Operating Systems I

* Description:	OTP Client-Server Project: Specifications

*****************************************************************/
#ifndef HELPERS_H
#define HELPERS_H

#define MAX_BUFFER 16

enum Cipher { ENC = 5111, DEC = 6222 };

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

// AUTHENTICATION
#include "authenticate.c"
void error(const char *msg);
void errorNoExit(const char* msg);

void authenticateServer(int fd, char* port, enum Cipher flag);
int authenticateClient(int fd, char* host, char* port);
// FILE RELATED
#include "fileHelpers.c"
int isValidValue(char* str, int length);
char* _initBuffer(int size);
int getFileSize(char* file);
void readFileLoop(char* file, char* buffer, int fileSize);
char* writeFileToBuffer(char* file, int* fileBytes);

// TRANSMISSION RELATED
#include "sendAndRecv.c"
int sendSize(int fd, int* bytesToSend);
int recvSize(int fd);
int sendall(int fd, char* msg, int* len);
char* recvAll(int fd, int* expected);

#endif
