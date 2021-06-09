CC=gcc
all:server client
server : server.c coroutine.c
	$(CC) $^ -o $@
client : client.c 
	$(CC) $^ -o $@
