#include "client.h"

volatile sig_atomic_t donec = 0;

void client(char* address){

	pthread_t r, s;
	int client_d;
	struct sockaddr_in server, client;
	struct paramc args;

	if((client_d = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		perror("\nsocket");
		exit(-1);
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr(address);
	bzero(server.sin_zero, 8);

	args.server = &server;
	args.client_d = &client_d;

	client.sin_family = AF_INET;
	client.sin_port = htons(PORT);
	client.sin_addr.s_addr = INADDR_ANY;
	bzero(client.sin_zero, 8);

	if(bind(client_d, (struct sockaddr*)&client, sizeof(struct sockaddr)) == -1){
		perror("\nbind");
		exit(-1);
	}

	pthread_create(&r, NULL, receivec, (void*)&args);
	pthread_create(&s, NULL, sendc, (void*)&args);
	pthread_join(s, NULL);
	pthread_join(r, NULL);

}

void* receivec(void* data){

	struct paramc* args = data;
	struct sockaddr_in* server = args->server;
	int* client_d = args->client_d;
	unsigned int length = sizeof(struct sockaddr_in);
	char* buffer = malloc(sizeof(char)*BUFFER_MAX);

	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = termc;
	sigaction(SIGINT, &action, NULL);

	while(!donec){

		if(recvfrom(*client_d, buffer, BUFFER_MAX, 0, (struct sockaddr*)server, &length) > 0){
			decrypt(buffer);
			if(*buffer != '['){
                printf("[%s]:%s", inet_ntoa(server->sin_addr), buffer);
			}else{
                printf("%s", buffer);
			}
			bzero(buffer, BUFFER_MAX);
		}

	}
	free(buffer);
	buffer = NULL;
    shutdown(*client_d, 2);
	printf("Client terminated (recievec).\n");

	return NULL;
}

void* sendc(void* data){

	struct paramc* args = data;
	struct sockaddr_in* server = args->server;
	int* client_d = args->client_d;
	char* buffer = malloc(sizeof(char)*BUFFER_MAX);

	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = termc;
	sigaction(SIGINT, &action, NULL);

	while(!donec){

		bzero(buffer, BUFFER_MAX);
		fgets(buffer, BUFFER_MAX, stdin);

		encrypt(buffer);

		sendto(*client_d, buffer, BUFFER_MAX, 0, (struct sockaddr*)server, sizeof(struct sockaddr_in));

	}
	free(buffer);
	buffer = NULL;
    shutdown(*client_d, 2);
	printf("Client terminated (sendc).\n");

	return NULL;
}

void termc(int signum){

	donec = 1;

}
