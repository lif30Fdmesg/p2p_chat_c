#include "server.h"
#include "linkedList.h"

//#define debug

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
	pthread_join(r, NULL);

}

void* receives(void* data){

	struct params* args = data;
	int* server_d = args->server_d;
	struct sockaddr_in* client = args->client;
	linkedList** iter = args->iter;
	unsigned int length = sizeof(struct sockaddr_in);
	char* buffer = malloc(sizeof(char)*BUFFER_MAX);
	struct sockaddr_in tmp;


	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = terms;
	sigaction(SIGINT, &action, NULL);

	bzero(buffer, BUFFER_MAX);

	while(!dones){

        if(recvfrom(*server_d, buffer, BUFFER_MAX, 0, (struct sockaddr*)client, &length) > 0){
            decrypt(buffer);
            printf("[%s]:%s", inet_ntoa(client->sin_addr), buffer);

            if(checkList(inet_ntoa(client->sin_addr)) != 1){
                addList(*client);
                beginList();
                #ifdef debug
                    printf("[DEBUG]clients IP is stored. size of LL: %d\n", sizeList());
                #endif
            }
            if(sizeList() > 1){
                beginList();
                for(int i = 0; i < sizeList(); i++){
                    #ifdef debug
                        printf("[DEBUG]Stored IP[%d]: %s\n", i, (*iter)->address);
                    #endif
                    nextList();
                    }
            }
            combine(inet_ntoa(client->sin_addr), &buffer);
            encrypt(buffer);
            beginList();
            for(int i = 0; i < sizeList(); i++){
                if (strcmp((*iter)->address, inet_ntoa(client->sin_addr)) != 0){
                      tmp.sin_addr.s_addr = inet_addr((*iter)->address);
                      tmp.sin_family = AF_INET;
                      tmp.sin_port = htons(PORT);
                      bzero(tmp.sin_zero, 8);
                      #ifdef debug
                            printf("[DEBUG]there is a client to send a message to. message: %s", buffer);
                            printf("[DEBUG]resend: %s --> %s\n",inet_ntoa(client->sin_addr), (*iter)->address);
                            printf("[DEBUG]IP in client->sin_addr: %s  port: %d\n", inet_ntoa(client->sin_addr), htons(client->sin_port));
                            printf("[DEBUG]message send: %d\n", (int)sendto(*server_d, buffer, BUFFER_MAX - 1, 0, (struct sockaddr*)&tmp, sizeof(struct sockaddr_in)));
                        #else
                            sendto(*server_d, buffer, BUFFER_MAX, 0, (struct sockaddr*)&tmp, sizeof(struct sockaddr_in));
                        #endif
                    nextList();
                }else{
                    nextList();
                }
            }

        }
        bzero(buffer, BUFFER_MAX);

	}
	free(buffer);
	buffer = NULL;
    shutdown(*server_d, 2);
    clearList();
	printf("Server terminated (recieves).\n");

	return NULL;
}

void* sends(void* data){

	struct params* args = data;
	int* server_d = args->server_d;
	struct sockaddr_in* client = args->client;
	linkedList** iter = args->iter;
	char* buffer = malloc(sizeof(char)*BUFFER_MAX);
	struct sockaddr_in tmp;

	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = terms;
	sigaction(SIGINT, &action, NULL);

	while(!dones){

		bzero(buffer, BUFFER_MAX);
		fgets(buffer, BUFFER_MAX, stdin); //tu sa sekne pri SIG_INT, to iste client

		encrypt(buffer);

        tmp.sin_addr.s_addr = client->sin_addr.s_addr;
        tmp.sin_family = AF_INET;
        tmp.sin_port = htons(PORT);
        bzero(tmp.sin_zero, 8);

        beginList();
        for(int i = 0; i < sizeList(); i++){
            tmp.sin_addr.s_addr = inet_addr((*iter)->address);
            #ifdef debug
                printf("[DEBUG]sending to address: %s\n", inet_ntoa(tmp.sin_addr));
            #endif
            sendto(*server_d, buffer, BUFFER_MAX, 0, (struct sockaddr*)&tmp, sizeof(struct sockaddr_in));
            nextList();
        }

	}
	free(buffer);
	buffer = NULL;
    shutdown(*server_d, 2);
	printf("Server terminated (sends).\n");

	return NULL;
}

void terms(int signum){

	dones = 1;

}

void combine(char* address, char** message){

    char* newBuffer = malloc(sizeof(char)*BUFFER_MAX);

    *newBuffer = '\0';
    strcat(newBuffer,"[");
    strcat(newBuffer, address);
    strcat(newBuffer, "]:");
    strcat(newBuffer, *message);

    free(*message);
    *message = newBuffer;

}
