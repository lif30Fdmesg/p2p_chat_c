start:	server.c client.c protocol.c encrypt.c main.c linkedList.c
	gcc server.c client.c protocol.c encrypt.c main.c linkedList.c -lpthread -o start
