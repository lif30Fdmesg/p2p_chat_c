#include "encrypt.h"
#include "const.h"
#include <stdio.h>

void encrypt(char* data){
    char *ch;
    ch = data;
    while(*ch!=10){
        #ifdef debug
            printf("[DEBUG]transforming asci: \'%c\' --> ", *ch);
        #endif
        *ch = *ch + 1;
        #ifdef debug
            printf("\'%c\'\n", *ch);
        #endif
        ch++;
    }
}

void decrypt(char* data){
    char *ch;
    ch = data;
    while(*ch!=10){
        #ifdef debug
        printf("[DEBUG]transforming asci: \'%c\' --> ", *ch);
        #endif
        *ch = *ch - 1;
        #ifdef debug
        printf("\'%c\'\n", *ch);
        #endif
        ch++;
    }
}
