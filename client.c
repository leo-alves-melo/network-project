#include <stdio.h>
#include <stdlib.h>
#include "bd.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close

#define STUDENT 'S'
#define PROFESSOR 'P'

#define ERROR -1
#define OK 0

#define RECEIVED_MESSAGE_LENGHT 1024*10

char checkClientType();
void studentConnection();
void professorConnection();
int logInStudent();
int logInProfessor();

int main(int argc, char **argv) {

	char *IPServer;

	if(argc > 1) {
		IPServer = argv[1];
	}
	else {
		printf("Informe o IP do servidor:\n");
		IPServer = (char *) malloc(sizeof(char) * 50);
		scanf(" %s", IPServer);
	}

	printf("IP do servidor informado: %s\n", IPServer);

	char clientType = checkClientType();

	if(clientType == STUDENT) {
		studentConnection(IPServer);
	}
	else {
		professorConnection(IPServer);
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

int sendToTheServer(char *IPServer, char *message) {

	struct addrinfo hints;
	struct addrinfo *servinfo;
	int status;
	char ipstr[INET6_ADDRSTRLEN];
	int socket_number;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;  
	hints.ai_socktype = SOCK_DGRAM;
	//hints.ai_flags = AI_PASSIVE;     

	status = getaddrinfo(IPServer, "3490", &hints, &servinfo);

    socket_number = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

    //connect(socket_number, servinfo->ai_addr, servinfo->ai_addrlen);

    sendto(socket_number, message, strlen(message), 0, servinfo->ai_addr, servinfo->ai_addrlen);

    return socket_number;
}

int connectToTheServer(char *IPServer) {

	

    return 0;
}

void studentConnection(char *IPServer) {

	char command;
	char * message;
	char received_message[RECEIVED_MESSAGE_LENGHT];
	char discipline[10];



	int i;

	struct addrinfo hints;
	struct addrinfo *servinfo;
	int status;
	char ipstr[INET6_ADDRSTRLEN];
	int socket_number;
	int addr_len;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;  
	hints.ai_socktype = SOCK_DGRAM;
	//hints.ai_flags = AI_PASSIVE;     

	status = getaddrinfo(IPServer, "3490", &hints, &servinfo);

    socket_number = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	printf("Bem-vindo Aluno!\n\n");

	while(1) {
		printf("\nO que você gostaria de consultar? Digite o número correspondente:\n");
		printf("[1] listar todos os códigos de disciplinas com seus respectivos títulos\n");
		printf("[2] dado o código de uma disciplina, retornar a ementa\n");
		printf("[3] dado o código de uma disciplina, retornar todas as informações desta disciplina\n");
		printf("[4] listar todas as informações de todas as disciplinas\n");
		printf("[5] dado o código de uma disciplina, retornar o texto de comentário sobre a próxima aula\n\n");

		for(i = 0; i < RECEIVED_MESSAGE_LENGHT; i++) {
    		received_message[i] = '\0';
   		}

		scanf(" %c", &command);


		if(command == '1') {
			
			message = make_client_message(1, NULL, NULL);
			socket_number = sendto(socket_number, message, strlen(message), 0, servinfo->ai_addr, servinfo->ai_addrlen);
			addr_len = sizeof servinfo;
			printf("vamos\n");
			recvfrom(socket_number, received_message, RECEIVED_MESSAGE_LENGHT-1 , 0, (struct sockaddr *)&servinfo, &addr_len);
			printf("fomos:\n");
			printf("bnana:\n%s\n", received_message);
		    //recv(socket_number, received_message, RECEIVED_MESSAGE_LENGHT, 0);
		    parse_server_message(received_message);
		    printf("nada\n");
		    close(socket_number);

		} else if(command == '2') {

			printf("Escreva o nome da disciplina:\n");
			scanf(" %s", discipline);
			socket_number = connectToTheServer(IPServer);
			message = make_client_message(2, discipline, NULL);
			send(socket_number, message, strlen(message), 0);
		    recv(socket_number, received_message, RECEIVED_MESSAGE_LENGHT, 0);
		    parse_server_message(received_message);
		    close(socket_number);


		} else if(command == '3') {
			printf("Escreva o nome da disciplina:\n");
			scanf(" %s", discipline);
			socket_number = connectToTheServer(IPServer);
			message = make_client_message(3, discipline, NULL);
			send(socket_number, message, strlen(message), 0);
		    recv(socket_number, received_message, RECEIVED_MESSAGE_LENGHT, 0);
		    parse_server_message(received_message);
		    close(socket_number);

		} else if(command == '4') {
			socket_number = connectToTheServer(IPServer);
			message = make_client_message(4, NULL, NULL);
			send(socket_number, message, strlen(message), 0);
		    recv(socket_number, received_message, RECEIVED_MESSAGE_LENGHT, 0);
		    parse_server_message(received_message);
		    close(socket_number);

		} else if(command == '5') {

			printf("Escreva o nome da disciplina:\n");
			scanf(" %s", discipline);
			socket_number = connectToTheServer(IPServer);
			message = make_client_message(6, discipline, NULL);
			send(socket_number, message, strlen(message), 0);
		    recv(socket_number, received_message, RECEIVED_MESSAGE_LENGHT, 0);
		    parse_server_message(received_message);
		    close(socket_number);
		} else {
			printf("Ops! Este não é um número válido!\n");
		}

	}

}

void professorConnection(char * IPServer) {
	char command;
	char * message;
	char received_message[RECEIVED_MESSAGE_LENGHT];
	char discipline[10];
	char comment[RECEIVED_MESSAGE_LENGHT];

	int socket_number;
	int i, h;

	struct timespec ts1, ts2;

	printf("Bem-vindo Professor!\n\n");

	while(1) {
		printf("\nO que você gostaria de realizar? Digite o número correspondente:\n\n");
		printf("[1] listar todos os códigos de disciplinas com seus respectivos títulos\n");
		printf("[2] dado o código de uma disciplina, retornar a ementa\n");
		printf("[3] dado o código de uma disciplina, retornar todas as informações desta disciplina\n");
		printf("[4] listar todas as informações de todas as disciplinas\n");
		printf("[5] escrever um texto de comentário sobre a próxima aula de uma disciplina\n");
		printf("[6] dado o código de uma disciplina, retornar o texto de comentário sobre a próxima aula\n\n");

		for(i = 0; i < RECEIVED_MESSAGE_LENGHT; i++) {
    		received_message[i] = '\0';
   		}

		scanf(" %c", &command);	

		if(command == '1') {
			socket_number = connectToTheServer(IPServer);
			message = make_client_message(1, NULL, NULL);
			send(socket_number, message, strlen(message), 0);
		    recv(socket_number, received_message, RECEIVED_MESSAGE_LENGHT, 0);
		    parse_server_message(received_message);
		    close(socket_number);

		} else if(command == '2') {

			printf("Escreva o nome da disciplina:\n");
			scanf(" %s", discipline);
			socket_number = connectToTheServer(IPServer);
			message = make_client_message(2, discipline, NULL);
			send(socket_number, message, strlen(message), 0);
		    recv(socket_number, received_message, RECEIVED_MESSAGE_LENGHT, 0);
		    parse_server_message(received_message);
		    close(socket_number);


		} else if(command == '3') {
			printf("Escreva o nome da disciplina:\n");
			scanf(" %s", discipline);
			socket_number = connectToTheServer(IPServer);
			message = make_client_message(3, discipline, NULL);
			send(socket_number, message, strlen(message), 0);
		    recv(socket_number, received_message, RECEIVED_MESSAGE_LENGHT, 0);
		    parse_server_message(received_message);
		    close(socket_number);

		} else if(command == '4') {
			socket_number = connectToTheServer(IPServer);
			message = make_client_message(4, NULL, NULL);
			send(socket_number, message, strlen(message), 0);
		    recv(socket_number, received_message, RECEIVED_MESSAGE_LENGHT, 0);
		    parse_server_message(received_message);
		    close(socket_number);

		} else if(command == '5') {

			printf("Escreva o nome da disciplina:\n");
			for(i = 0; i < RECEIVED_MESSAGE_LENGHT; i++) {
    			comment[i] = '\0';
 			}
			scanf(" %s", discipline);
			printf("Escreva o comentário:\n");
			scanf(" %[^\n]", comment);
			printf("comentario: %s\n", comment);
			socket_number = connectToTheServer(IPServer);
			message = make_client_message(5, discipline, comment);
			send(socket_number, message, strlen(message), 0);
		    recv(socket_number, received_message, RECEIVED_MESSAGE_LENGHT, 0);
		    printf("Mensagem escrita com sucesso!\n");
		    close(socket_number);

		} else if(command == '6') {

			printf("Escreva o nome da disciplina:\n");
			scanf(" %s", discipline);
			socket_number = connectToTheServer(IPServer);
			message = make_client_message(6, discipline, NULL);
			send(socket_number, message, strlen(message), 0);
		    recv(socket_number, received_message, RECEIVED_MESSAGE_LENGHT, 0);
		    parse_server_message(received_message);
		    close(socket_number);
		}else {
			printf("Ops! Este não é um número válido!\n");
		}


		

	}

	while(logInStudent() == ERROR) {
		printf("Erro de conexão! Tentando novamente...\n");
	}
}
