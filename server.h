#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include "const.h"
#include "encrypt.h"
#include "linkedList.h"

struct params{
	struct sockaddr_in* client;
	int* server_d;
	linkedList** iter;
};

volatile sig_atomic_t dones;

void server();
void* receives(void*);
void* sends(void*);
void terms(int);
void combine(char*, char**);

#endif
