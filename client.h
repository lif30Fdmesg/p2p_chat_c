#ifndef CLIENT_H_
#define CLIENT_H_

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

struct paramc{
	struct sockaddr_in* server;
	int* client_d;
};

volatile sig_atomic_t donec;

void client(char*);
void* receivec(void*);
void* sendc(void*);
void termc(int);

#endif
