#include "protocol.h"


int getID(unsigned char* id){

    unsigned char ret = 0;

    ret += (*id>>2 & 1)*1;
    ret += (*id>>3 & 1)*2;
    ret += (*id>>4 & 1)*4;
    ret += (*id>>5 & 1)*8;
    ret += (*id>>6 & 1)*16;
    ret += (*id>>7 & 1)*32;
    return (int)ret;
}

int getProtocol(unsigned char* prot){

    unsigned char ret = 0;

    ret += (*prot & 1)*1;
    ret += (*prot>>1 & 1)*2;
    return (int)ret;
}

int setID(unsigned char* id, int nID){

    int check = nID;
    int* tempP = (int*) malloc(sizeof(int)*2);
    *tempP = (int)*id & 1;
    *(tempP+1) = (int)*id >> 1 & 1;

    int* tempID = (int*) malloc(sizeof(int)*6);

    *id = *id >> 2;

    for(int i = 0; i < 6; i++){

        *(tempID+i) = nID%2;
        nID /= 2;

    }

    if(nID != 0) return -1;

    bzero(id, 1);

    for(int i = 5; i > -1; i--){
        *id = *id | *(tempID+i);
        *id = *id<<1;
    }
    for(int i = 1; i > -1; i--){
        *id = *id | *(tempP+i);
         if (i != 0) *id = *id<<1;
    }

    free(tempP);
    free(tempID);
    tempP = NULL;
    tempID = NULL;

    if(check == getID(id)) return 0; else return -1;

}

int setProtocol(unsigned char* protocol, int pID){

    int* temp = (int*) malloc(sizeof(int)*2);
    int check = pID;

    for(int i = 0; i < 2; i++){

        *(temp+i) = pID%2;
        pID /= 2;

    }

    if(pID != 0) return -1;

    *protocol = *protocol & 0;
    *protocol = *protocol >> 1;
    *protocol = *protocol & 0;

    *protocol = *protocol | *(temp+1);
    *protocol = *protocol << 1;
    *protocol = *protocol | *temp;

    free(temp);
    temp = NULL;

    if(check == (int)getProtocol(protocol)) return 0; else return -1;
}
