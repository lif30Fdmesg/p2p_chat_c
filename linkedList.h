#ifndef LINKED_LIST_
#define LINKED_LIST_

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

typedef struct ll{

    int socket;
    char* address;
    struct ll* next;
    struct ll* prev;

} linkedList;


linkedList* initList(struct sockaddr_in);
linkedList* addList(struct sockaddr_in);
linkedList* removeList(char*);
unsigned short int clearList();
unsigned short int sizeList();
linkedList* nextList();
linkedList* prevList();
linkedList* beginList();
linkedList* endList();
linkedList** getIteratorList();
unsigned short int checkList(char*);


#endif
