#include<stdio.h>
#include "bd.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define FILENAME "data.json"
#define CLIENTS 30

#define MESSAGE_LENGHT 1024*10

cJSON* db;

typedef struct Client {
	int socket_number;
} Client;

void respondClient(void * address) {
	char mess[MESSAGE_LENGHT];
	char *retorno;

	Client *client = (Client *) address;


    for(int i = 0; i < MESSAGE_LENGHT; i++) {
    	mess[i] = '\0';
    }

    recv(client->socket_number, mess, MESSAGE_LENGHT, 0);

    printf("Messagem que recebi: \n%s\n", mess);

    parse_client_message(mess, &retorno, db);

    send(client->socket_number, retorno, strlen(retorno), 0);

    close(client->socket_number);
}

int main() {

	char command;
	char * message;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	int status;
	char ipstr[INET6_ADDRSTRLEN];
	int socket_number, new_fd;
	int meu_bind;
	struct sockaddr_storage their_addr;
	socklen_t addr_size;
	FILE *file;
	Client * client;
	pthread_t * thread;

	printf("Oi, sou servidor\n");

	initialize(FILENAME, &file, &db);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

	status = getaddrinfo(NULL, "3490", &hints, &servinfo);

	if(status != 0) {
		printf("getaddrinfo error: %s\n", gai_strerror(status));
	}


	socket_number = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	meu_bind = bind(socket_number, servinfo->ai_addr, servinfo->ai_addrlen); //Colocar funcao de esperar se o BIND nao der certo

	while(meu_bind == -1) {
		printf("Erro ao conectar, tentando novamente em 5 segundos...\n");
		sleep(5);
		meu_bind = bind(socket_number, servinfo->ai_addr, servinfo->ai_addrlen); //Colocar funcao de esperar se o BIND nao der certo

	}
	printf("Olha meu bind: %d\n", meu_bind);

	listen(socket_number, CLIENTS);

	while(1) {


		addr_size = sizeof their_addr;

	    new_fd = accept(socket_number, (struct sockaddr *)&their_addr, &addr_size);

	    client = (Client *) malloc(sizeof(Client *));

	    client->socket_number = new_fd;

	    thread = (pthread_t *) malloc(sizeof(pthread_t *));
	    pthread_create(thread, NULL, (void *) respondClient, (void *) client);


	}




	return 0;
}
