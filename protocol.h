#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

int getID(unsigned char*);
int getProtocol(unsigned char*);
int setID(unsigned char*, int);
int setProtocol(unsigned char*, int);

#endif

