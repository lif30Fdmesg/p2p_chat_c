#include "server.h"
#include "linkedList.h"

volatile sig_atomic_t dones = 0;

void server(){
	pthread_t r, s;
	int server_d;
	struct sockaddr_in server, client;
	struct params args;

    linkedList** iter = getIteratorList();

	if((server_d = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		perror("\nsocket");
		exit(-1);
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(server.sin_zero, 8);

	args.client = &client;
	args.server_d = &server_d;
	args.iter = iter;

	if(bind(server_d, (struct sockaddr*)&server, sizeof(struct sockaddr)) == -1){
		perror("\nbind");
		exit(-1);
	}

	pthread_create(&r, NULL, receives, (void*)&args);
	pthread_create(&s, NULL, sends, (void*)&args);
	pthread_join(s, NULL);

}

void* receives(void* data){

	int flag = 0;
	struct params* args = data;
	int* server_d = args->server_d;
	struct sockaddr_in* client = args->client;
	linkedList** iter = args->iter;
	unsigned int length = sizeof(struct sockaddr_in);
	char buffer[BUFFER_MAX];
	struct sockaddr_in tmp;


	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = terms;
	sigaction(SIGINT, &action, NULL);

	bzero(buffer, BUFFER_MAX);

	while(!dones){

		if(flag == 0){
			if(recvfrom(*server_d, buffer, BUFFER_MAX - 1, 0, (struct sockaddr*)client, &length) > 0){
				flag = 1;
				decrypt(buffer);
				printf("[%s]:%s", inet_ntoa(client->sin_addr), buffer);


				//some new staff





				if(checkList(inet_ntoa(client->sin_addr)) != 1){
                    addList(*client);
                    printf("[DEBUG]clients IP is stored. size of LL: %d\n", sizeList());
				}
				if(sizeList() > 1){
                    beginList();
                    for(int i = 0; i < sizeList(); i++){
                        printf("[DEBUG]Stored IP[%d]: %s\n", i, (*iter)->address);
                        nextList();
                        }
                }
                    beginList();
                    for(int i = 0; i < sizeList(); i++){
                        if (strcmp((*iter)->address, inet_ntoa(client->sin_addr)) != 0){

                            printf("[DEBUG]there is a client to send a message to. message: %s", buffer);

                            printf("[DEBUG]resend: %s --> %s\n",inet_ntoa(client->sin_addr), (*iter)->address);

                            tmp.sin_addr.s_addr = inet_addr((*iter)->address);
                            tmp.sin_family = client->sin_family;
                            tmp.sin_port = htons(PORT);
                            bzero(tmp.sin_zero, 8);

                            printf("[DEBUG]IP in tmp.sin_addr: %s\n", inet_ntoa(tmp.sin_addr));
                            printf("[DEBUG]IP in client->sin_addr: %s  port: %d\n", inet_ntoa(client->sin_addr), htons(client->sin_port));

                            printf("[DEBUG]message send: %d\n", (int)sendto(*server_d, buffer, BUFFER_MAX - 1, 0, (struct sockaddr*)&tmp, sizeof(struct sockaddr_in)));
                            nextList();
                        }else{
                            nextList();
                        }
                    }










				// end of new staff

			}
			bzero(buffer, BUFFER_MAX);
		}else{
			if(recvfrom(*server_d, buffer, BUFFER_MAX - 1, 0, (struct sockaddr*)client, &length) == 0){
				flag = 0;
			}else{
				decrypt(buffer);
				printf("[%s]:%s", inet_ntoa(client->sin_addr), buffer);





				//some new staff












				if(checkList(inet_ntoa(client->sin_addr)) != 1){
                    addList(*client);
                    printf("[DEBUG]clients IP is stored. size of LL: %d\n", sizeList());
				}
				if(sizeList() > 1){
                    beginList();
                    for(int i = 0; i < sizeList(); i++){
                        printf("[DEBUG]Stored IP[%d]: %s\n", i, (*iter)->address);
                        nextList();
                        }
                }
                    beginList();
                    for(int i = 0; i < sizeList(); i++){
                        if (strcmp((*iter)->address, inet_ntoa(client->sin_addr)) != 0){

                            printf("[DEBUG]there is a client to send a message to. message: %s", buffer);

                            printf("[DEBUG]resend: %s --> %s\n",inet_ntoa(client->sin_addr), (*iter)->address);

                            tmp.sin_addr.s_addr = inet_addr((*iter)->address);
                            tmp.sin_family = client->sin_family;
                            tmp.sin_port = htons(PORT);
                            bzero(tmp.sin_zero, 8);

                            printf("[DEBUG]IP in tmp.sin_addr: %s\n", inet_ntoa(tmp.sin_addr));
                            printf("[DEBUG]IP in client->sin_addr: %s  port: %d\n", inet_ntoa(client->sin_addr), htons(client->sin_port));

                            printf("[DEBUG]message send: %d\n", (int)sendto(*server_d, buffer, BUFFER_MAX - 1, 0, (struct sockaddr*)&tmp, sizeof(struct sockaddr_in)));
                            nextList();
                        }else{
                            nextList();
                        }
                    }










				// end of new staff

			}
			bzero(buffer, BUFFER_MAX);
		}

	}

	printf("Server terminated.\n");

	return NULL;
}

void* sends(void* data){

	struct params* args = data;
	int* server_d = args->server_d;
	struct sockaddr_in* client = args->client;
	linkedList** iter = args->iter;
	char buffer[BUFFER_MAX];
	struct sockaddr_in tmp;

	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = terms;
	sigaction(SIGINT, &action, NULL);

	while(!dones){

		bzero(buffer, BUFFER_MAX);
		fgets(buffer, BUFFER_MAX - 1, stdin);

		encrypt(buffer);
/*
		if(sendto(*server_d, buffer, BUFFER_MAX - 1, 0, (struct sockaddr*)client, sizeof(struct sockaddr_in)) == -1){
			printf("Message was not sent...\n");
		}
*/


        tmp.sin_addr.s_addr = client->sin_addr.s_addr;
        tmp.sin_family = client->sin_family;
        tmp.sin_port = client->sin_port;
        bzero(tmp.sin_zero, 8);

        beginList();
        for(int i = 0; i < sizeList(); i++){
            tmp.sin_addr.s_addr = inet_addr((*iter)->address);
            printf("[DEBUG]sending to address: %s\n", inet_ntoa(tmp.sin_addr));
            sendto(*server_d, buffer, BUFFER_MAX - 1, 0, (struct sockaddr*)&tmp, sizeof(struct sockaddr_in));
            nextList();
        }

	}

	printf("Server terminated.\n");

	return NULL;
}

void terms(int signum){

	dones = 1;
	if(clearList() == 0) printf("\n\nLinkedList was succesfully deleted.\n");
	printf("\nTerminating server...\n");

}
