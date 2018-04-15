#include<stdio.h>
#include "bd.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#define FILENAME "data.json"

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

	cJSON* db;

	printf("Oi, sou servidor\n");

	initialize(FILENAME, &file, &db);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

	printf("oi\n");

	status = getaddrinfo(NULL, "3490", &hints, &servinfo);

	if(status != 0) {
		printf("getaddrinfo error: %s\n", gai_strerror(status));
	}


	socket_number = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	printf("oi\n");

	meu_bind = bind(socket_number, servinfo->ai_addr, servinfo->ai_addrlen);

	printf("Olha meu bind: %d\n", meu_bind);

	listen(socket_number, 1);

	addr_size = sizeof their_addr;
    new_fd = accept(socket_number, (struct sockaddr *)&their_addr, &addr_size);

    char mess[513];
    recv(new_fd, mess, 512, 0);

    printf("Messagem que recebi: %s\n", mess);

    char *retorno;
    parse_client_message(mess, &retorno, db);

    send(new_fd, retorno, strlen(retorno), 0);

    close(new_fd); 



	return 0;
}
