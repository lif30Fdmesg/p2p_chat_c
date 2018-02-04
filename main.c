#include "const.h"

#ifdef linkedlist_test
    #include "linkedList.h"
#else
    #include "server.h"
    #include "client.h"
#endif

int main(int argc, char* argv[]){


#ifndef linkedlist_test
	system("clear");

	if(argc < 2){

		server();

	}else{

		client(argv[1]);

	}
#endif
#ifdef linkedlist_test
	struct sockaddr_in test1;
	test1.sin_family = AF_INET;
	test1.sin_port = htons(PORT);
	test1.sin_addr.s_addr = inet_addr("192.168.1.101");
	bzero(test1.sin_zero, 8);

	struct sockaddr_in test2;
	test2.sin_family = AF_INET;
	test2.sin_port = htons(PORT);
	test2.sin_addr.s_addr = inet_addr("192.168.1.102");
	bzero(test2.sin_zero, 8);

	struct sockaddr_in test3;
	test3.sin_family = AF_INET;
	test3.sin_port = htons(PORT);
	test3.sin_addr.s_addr = inet_addr("192.168.1.103");
	bzero(test3.sin_zero, 8);

	struct sockaddr_in test4;
	test4.sin_family = AF_INET;
	test4.sin_port = htons(PORT);
	test4.sin_addr.s_addr = inet_addr("192.168.1.104");
	bzero(test4.sin_zero, 8);

	struct sockaddr_in test5;
	test5.sin_family = AF_INET;
	test5.sin_port = htons(PORT);
	test5.sin_addr.s_addr = inet_addr("192.168.1.105");
	bzero(test5.sin_zero, 8);

	linkedList** iterator = getIteratorList();
	printf("address: %p/%p\torder: %d\n", *iterator, addList(test1), sizeList());
	printf("address: %p/%p\torder: %d\n", *iterator, addList(test2), sizeList());
	printf("address: %p/%p\torder: %d\n", *iterator, addList(test3), sizeList());
	printf("address: %p/%p\torder: %d\n", *iterator, addList(test4), sizeList());
	printf("address: %p/%p\torder: %d\n", *iterator, addList(test5), sizeList());

	printf("vypis testovych struktur priamo:\n%s\n", inet_ntoa(test1.sin_addr));
	printf("%s\n", inet_ntoa(test2.sin_addr));
	printf("%s\n", inet_ntoa(test3.sin_addr));
	printf("%s\n", inet_ntoa(test4.sin_addr));
	printf("%s\n", inet_ntoa(test5.sin_addr));

	printf("========================\n");
	checkList("192.168.1.104");
	printf("checkList() test: adresa -//-.104 sa nachadza: %p, plna adresa je %s\n", (*iterator), (*iterator)->address);

    beginList();
	for(int i = 0; i < sizeList(); i++){
        printf("[%p]list\'s output [%d]: %s\n", *iterator, i, (*iterator)->address);
        nextList();
	}
	printf("========================\n");

	removeList("192.168.1.102");
	printf("deleting 192.168.1.102\n");
	printf("size: %d\n", sizeList());


    beginList();
	for(int i = 0; i < sizeList(); i++){
        printf("[%p]list\'s output [%d]: %s\n", *iterator, i, (*iterator)->address);
        nextList();
	}
	printf("========================\n");
	removeList("192.168.1.101");
	printf("deleting 192.168.1.101\n");
	printf("size: %d\n", sizeList());

    beginList();
	for(int i = 0; i < sizeList(); i++){
        printf("[%p]list\'s output [%d]: %s\n", *iterator, i, (*iterator)->address);
        nextList();
	}

	printf("========================\n");
	removeList("192.168.1.105");
	printf("deleting 192.168.1.105\n");
	printf("size: %d\n", sizeList());



    beginList();
	for(int i = 0; i < sizeList(); i++){
        printf("[%p]list\'s output [%d]: %s\n", *iterator, i, (*iterator)->address);
        nextList();
	}
	printf("========================\n");

	printf("removing the whole list\n");
	if(clearList() == 0) printf("successfully removed\n"); else printf("some chiki-briki is not right. FAILUREEE!");
	printf("size: %d\n", sizeList());
	printf("========================\n");
#endif
	return 0;
}
