#include<stdio.h>
#include "bd.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#define STUDENT 'S'
#define PROFESSOR 'P'

#define ERROR -1
#define OK 0

char checkClientType();
void studentConnection();
void professorConnection();
int logInStudent();
int logInProfessor();

int main() {

	char clientType = checkClientType();
	
	if(clientType == STUDENT) {
		studentConnection();
	}
	else {
		professorConnection();
	}

	return 0;
}

char checkClientType() {
	char clientType = 0;

	printf("Bem vindo ao sistema de comunicação Alunos-Professor! Digite [A] se você for aluno ou [P] se você for professor:\n");

	while(clientType == 0) {
		scanf(" %c", &clientType);

		if(clientType == 'A' || clientType == 'a') {
			clientType = STUDENT;
		}
		else if(clientType == 'P' || clientType == 'p') {
			clientType = PROFESSOR;
		}
		else {
			printf("Não entendi, Digite [A] se você for aluno ou [P] se você for professor:\n");
			clientType = 0;
		}
	}

	return clientType;
}

int connectToTheServer() {

	struct addrinfo hints;
	struct addrinfo *servinfo;
	int status;
	char ipstr[INET6_ADDRSTRLEN];
	int socket_number;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

	status = getaddrinfo("127.0.0.1", "3490", &hints, &servinfo);

	printf("Olha o meu status: %d\n", status);

	//for(struct addrinfo *p = servinfo;p != NULL; p = p->ai_next) {
    //    void *addr;
    //    char *ipver;
//
    //    // get the pointer to the address itself,
    //    // different fields in IPv4 and IPv6:
    //    if (p->ai_family == AF_INET) { // IPv4
    //        struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
    //        addr = &(ipv4->sin_addr);
    //        ipver = "IPv4";
    //    } else { // IPv6
    //        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
    //        addr = &(ipv6->sin6_addr);
    //        ipver = "IPv6";
    //    }
//
    //    // convert the IP to a string and print it:
    //    inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    //    printf("  %s: %s\n", ipver, ipstr);
    //}

    socket_number = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    printf("olha meu soquéte: %d\n", socket_number);

    printf("Conectei? %d\n", connect(socket_number, servinfo->ai_addr, servinfo->ai_addrlen));

    return socket_number;
}

void studentConnection() {

	char command;
	char * message;
	char received_message[513];
	char discipline[10];

	int socket_number;

	printf("Bem-vindo Aluno!\n\n");

	while(1) {
		printf("O que você gostaria de consultar? Digite o número correspondente:\n");
		printf("[1] listar todos os códigos de disciplinas com seus respectivos títulos\n");
		printf("[2] dado o código de uma disciplina, retornar a ementa\n");
		printf("[3] dado o código de uma disciplina, retornar todas as informações desta disciplina\n");
		printf("[4] listar todas as informações de todas as disciplinas\n");
		printf("[5] dado o código de uma disciplina, retornar o texto de comentário sobre a próxima aula\n");

		scanf(" %c", &command);

		socket_number = connectToTheServer();

		if(command == '1') {
			message = make_client_message(1, NULL, NULL);
			send(socket_number, message, strlen(message), 0); 
		    recv(socket_number, received_message, 512, 0);
		    parse_server_message(received_message);

		} else if(command == '2') {
			printf("Escreva o nome da disciplina:\n");
			scanf(" %s", discipline);
			message = make_client_message(2, discipline, NULL);
			send(socket_number, message, strlen(message), 0); 
		    recv(socket_number, received_message, 512, 0);
		    parse_server_message(received_message);


		} else if(command == '3') {

		} else if(command == '4') {

		} else if(command == '5') {

		} else {

		}

		close(socket_number); 
	}

	while(logInStudent() == ERROR) {
		printf("Erro de conexão! Tentando novamente...\n");
	}

}

void professorConnection() {
	while(logInProfessor() == ERROR) {
		printf("Erro de conexão! Tentando novamente...\n");
	}
}

int logInStudent() {

	char * message = make_client_message(0, "MC102", "Eu me amo");

	printf("%s\n", message);
	return OK;
}

int logInProfessor() {
	printf("Conectando como professor...\n");
	return ERROR;
}