#include <stdio.h>
#include "bd.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define FILENAME "data.json"
#define CLIENTS 30

#define MESSAGE_LENGHT 1024*10

void *get_in_addr_mine(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main() {

	char command;
	char * message;
	struct addrinfo hints, hints_send;
	struct addrinfo *servinfo, *servinfo_send;
	int status;
	char ipstr[INET6_ADDRSTRLEN];
	int socket_number, socket_number_send, new_fd;
	int meu_bind;
	struct sockaddr_in their_addr;
	socklen_t addr_size;
	FILE *file;
	struct timespec ts1, ts2;
	char message_buffer[MESSAGE_LENGHT];
	int addr_len, addr_len_send;
	int i;
	char *retorno;
	char s[INET_ADDRSTRLEN];

	cJSON* db;

	printf("Oi, sou servidor\n");

	initialize(FILENAME, &file, &db);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; 
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;     

	status = getaddrinfo(NULL, "3490", &hints, &servinfo);

	if(status != 0) {
		printf("getaddrinfo error: %s\n", gai_strerror(status));
	}




	socket_number = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	while(socket_number == -1) {
		printf("Erro ao fazer o socket, tentando novamente em 5 segundos...\n");
		sleep(5);
		socket_number = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	}

	meu_bind = bind(socket_number, servinfo->ai_addr, servinfo->ai_addrlen); //Colocar funcao de esperar se o BIND nao der certo

	while(meu_bind == -1) {
		printf("Erro ao fazer o bind, tentando novamente em 5 segundos...\n");
		sleep(5);
		meu_bind = bind(socket_number, servinfo->ai_addr, servinfo->ai_addrlen); //Colocar funcao de esperar se o BIND nao der certo

	}
	printf("Olha meu bind: %d\n", meu_bind);

	
	freeaddrinfo(servinfo);

	while(1) {

		for(i = 0; i < MESSAGE_LENGHT; i++) {
			message_buffer[i] = '\0';
		}

		addr_len = sizeof their_addr;
		recvfrom(socket_number, message_buffer, MESSAGE_LENGHT-1 , 0, (struct sockaddr_in *) &their_addr, &addr_len);
		printf("Olha a mensagem que recebi: \n %s\n", message_buffer);

		parse_client_message(message_buffer, &retorno, db);
		printf("Vou enviar isso: %s\n", retorno);

		
		addr_len = sizeof their_addr;
		sendto(socket_number, retorno, strlen(retorno), 0, (struct sockaddr_in *) &their_addr, addr_len);

		//clock_gettime(CLOCK_REALTIME, &ts1);

		/*
	    client = (Client *) malloc(sizeof(Client));


	    client->socket_number = new_fd;
	    client->db = db;
	    client->lock = &lock;
		client->time = &ts1;


	    thread = (pthread_t *) malloc(sizeof(pthread_t));

	    pthread_create(thread, NULL, (void *) respondClient, (void *) client);
	    */


	}




	return 0;
}
