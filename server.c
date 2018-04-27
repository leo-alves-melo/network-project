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

#define FILENAME "data.json"
#define CLIENTS 30

#define MESSAGE_LENGHT 1024*10


typedef struct Client {
	int socket_number;
	cJSON* db;
	pthread_mutex_t *lock;
	struct timespec *time;

} Client;

void respondClient(void * address) {
	char mess[MESSAGE_LENGHT];
	char *retorno;
	cJSON* db;
	int i;
	struct timespec ts1, ts2;

	Client *client = (Client *) address;
	db = client->db;


    for(i = 0; i < MESSAGE_LENGHT; i++) {
    	mess[i] = '\0';
    }


    recv(client->socket_number, mess, MESSAGE_LENGHT, 0);

    /* Deve-se tomar cuidado com o parse, ele acessa recursos do banco de dados */

    pthread_mutex_lock(client->lock);

    parse_client_message(mess, &retorno, db);

    pthread_mutex_unlock(client->lock);


    if(retorno != NULL) {
    	send(client->socket_number, retorno, strlen(retorno), 0);
    }
    else {
    	send(client->socket_number, "{}", 3, 0);
    }

	clock_gettime(CLOCK_REALTIME, &ts1);
	printf("%lf\n", difftime(ts1.tv_nsec, client->time->tv_nsec));


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
	pthread_mutex_t lock;
	struct timespec ts1, ts2;

	cJSON* db;

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

	pthread_mutex_init(&lock, NULL);

	while(1) {


		addr_size = sizeof their_addr;

	    new_fd = accept(socket_number, (struct sockaddr *)&their_addr, &addr_size);

		clock_gettime(CLOCK_REALTIME, &ts1);


	    client = (Client *) malloc(sizeof(Client));


	    client->socket_number = new_fd;
	    client->db = db;
	    client->lock = &lock;
		client->time = &ts1;


	    thread = (pthread_t *) malloc(sizeof(pthread_t));

	    pthread_create(thread, NULL, (void *) respondClient, (void *) client);


	}




	return 0;
}
