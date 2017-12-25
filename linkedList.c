#include "linkedList.h"

static linkedList* iterator = NULL;

linkedList* initList(struct sockaddr_in p_new){

    iterator = (linkedList*) malloc(sizeof(linkedList));
    if(iterator == NULL) return NULL;
    iterator->next = NULL;
    iterator->prev = NULL;
    iterator->address = (char*)malloc(sizeof(char)*16);
    strcpy(iterator->address, inet_ntoa(p_new.sin_addr));
    return iterator;

}
linkedList* addList(struct sockaddr_in p_new){

    if(iterator == NULL){

        return initList(p_new);

    }
    endList();
    iterator->next = (linkedList*) malloc(sizeof(linkedList));
    if(iterator->next == NULL) return NULL;
    iterator->next->next = NULL;
    iterator->next->prev = iterator;
    iterator->next->address = (char*)malloc(sizeof(char)*16);
    strcpy(iterator->next->address, inet_ntoa(p_new.sin_addr));
    return iterator = iterator->next;

}
linkedList* removeList(char* p_address){

    beginList();
    for(int i = 0; i < sizeList(); i++){
        if(strcmp(iterator->address, p_address) == 0){

            if(iterator->prev == NULL){

                iterator = iterator->next;
                free(iterator->prev->address);
                free(iterator->prev);
                iterator->prev = NULL;
                return iterator;

            }else if(iterator->next == NULL){

                iterator = iterator->prev;
                free(iterator->next->address);
                free(iterator->next);
                iterator->next = NULL;
                return iterator;

            }else{

                linkedList* tmp = iterator->next;
                iterator->prev->next = iterator->next;
                iterator->next->prev = iterator->prev;
                free(iterator->address);
                free(iterator);
                return iterator = tmp;

            }


        }else{
            nextList();
        }
    }
    beginList();
    return iterator;
}
unsigned short int clearList(){

    if (iterator == NULL) return 404;

    endList();

    while( (iterator->next != NULL && iterator->prev != NULL) ){

        iterator = iterator->prev;
        free(iterator->next->address);
        free(iterator->next);
        iterator->next = NULL;

    }

    free(iterator);
    iterator = NULL;
    return 0;

}
unsigned short int sizeList(){

    unsigned short int size = 0;
    unsigned int tmp_range = 0;
    if(iterator == NULL) return size;
    while(iterator->prev != NULL){
        tmp_range++;
        prevList();
    }

    while(iterator->next != NULL){
        size++;
        nextList();
    }
    size++;
    beginList();
    for(int i = 0; i < tmp_range; i++){
        nextList();
    }
    return size;

}
linkedList* nextList(){

    if (iterator == NULL) return NULL;

    if(iterator->next == NULL) return iterator; else return iterator = iterator->next;

}
linkedList* prevList(){

    if (iterator == NULL) return NULL;

    if(iterator->prev == NULL) return iterator; else return iterator = iterator->prev;

}
linkedList* beginList(){

    if (iterator == NULL) return NULL;

    while(iterator->prev != NULL){
        prevList();
    }

    return iterator;

}
linkedList* endList(){

    if (iterator == NULL) return NULL;

    while(iterator->next != NULL){
        nextList();
    }

    return iterator;

}
linkedList** getIteratorList(){
    return &iterator;
}
unsigned short int checkList(char* p_str){

    if(iterator == NULL) return 0;

    beginList();
    for(int i = 0; i < sizeList(); i++){
        if(strcmp(iterator->address, p_str) == 0){

            return 1;

        }else nextList();
    }
    beginList();
    return 0;
}
