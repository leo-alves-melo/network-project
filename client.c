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

#define RECEIVED_MESSAGE_LENGHT 1024*10

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

	//printf("Olha o meu status: %d\n", status);

    socket_number = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

    connect(socket_number, servinfo->ai_addr, servinfo->ai_addrlen);

    return socket_number;
}

void studentConnection() {

	char command;
	char * message;
	char received_message[RECEIVED_MESSAGE_LENGHT];
	char discipline[10];

	int socket_number;

	printf("Bem-vindo Aluno!\n\n");

	while(1) {
		printf("\nO que você gostaria de consultar? Digite o número correspondente:\n");
		printf("[1] listar todos os códigos de disciplinas com seus respectivos títulos\n");
		printf("[2] dado o código de uma disciplina, retornar a ementa\n");
		printf("[3] dado o código de uma disciplina, retornar todas as informações desta disciplina\n");
		printf("[4] listar todas as informações de todas as disciplinas\n");
		printf("[5] dado o código de uma disciplina, retornar o texto de comentário sobre a próxima aula\n\n");

		for(int i = 0; i < RECEIVED_MESSAGE_LENGHT; i++) {
    		received_message[i] = '\0';
   		}

		scanf(" %c", &command);

		

		if(command == '1') {
			socket_number = connectToTheServer();
			message = make_client_message(1, NULL, NULL);
			send(socket_number, message, strlen(message), 0); 
		    recv(socket_number, received_message, RECEIVED_MESSAGE_LENGHT, 0);
		    parse_server_message(received_message);
		    close(socket_number); 

		} else if(command == '2') {

			printf("Escreva o nome da disciplina:\n");
			scanf(" %s", discipline);
			socket_number = connectToTheServer();
			message = make_client_message(2, discipline, NULL);
			send(socket_number, message, strlen(message), 0); 
		    recv(socket_number, received_message, RECEIVED_MESSAGE_LENGHT, 0);
		    parse_server_message(received_message);
		    close(socket_number); 


		} else if(command == '3') {
			printf("Escreva o nome da disciplina:\n");
			scanf(" %s", discipline);
			socket_number = connectToTheServer();
			message = make_client_message(3, discipline, NULL);
			send(socket_number, message, strlen(message), 0); 
		    recv(socket_number, received_message, RECEIVED_MESSAGE_LENGHT, 0);
		    parse_server_message(received_message);
		    close(socket_number); 

		} else if(command == '4') {
			socket_number = connectToTheServer();
			message = make_client_message(4, NULL, NULL);
			send(socket_number, message, strlen(message), 0); 
		    recv(socket_number, received_message, RECEIVED_MESSAGE_LENGHT, 0);
		    parse_server_message(received_message);
		    close(socket_number); 

		} else if(command == '5') {

			printf("Escreva o nome da disciplina:\n");
			scanf(" %s", discipline);
			socket_number = connectToTheServer();
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

void professorConnection() {
	char command;
	char * message;
	char received_message[RECEIVED_MESSAGE_LENGHT];
	char discipline[10];
	char comment[RECEIVED_MESSAGE_LENGHT];

	int socket_number;

	printf("Bem-vindo Professor!\n\n");

	while(1) {
		printf("\nO que você gostaria de realizar? Digite o número correspondente:\n\n");
		printf("[1] listar todos os códigos de disciplinas com seus respectivos títulos\n");
		printf("[2] dado o código de uma disciplina, retornar a ementa\n");
		printf("[3] dado o código de uma disciplina, retornar todas as informações desta disciplina\n");
		printf("[4] listar todas as informações de todas as disciplinas\n");
		printf("[5] escrever um texto de comentário sobre a próxima aula de uma disciplina\n");
		printf("[6] dado o código de uma disciplina, retornar o texto de comentário sobre a próxima aula\n\n");

		for(int i = 0; i < RECEIVED_MESSAGE_LENGHT; i++) {
    		received_message[i] = '\0';
   		}

		scanf(" %c", &command);

		

		if(command == '1') {
			socket_number = connectToTheServer();
			message = make_client_message(1, NULL, NULL);
			send(socket_number, message, strlen(message), 0); 
		    recv(socket_number, received_message, RECEIVED_MESSAGE_LENGHT, 0);
		    parse_server_message(received_message);
		    close(socket_number); 

		} else if(command == '2') {

			printf("Escreva o nome da disciplina:\n");
			scanf(" %s", discipline);
			socket_number = connectToTheServer();
			message = make_client_message(2, discipline, NULL);
			send(socket_number, message, strlen(message), 0); 
		    recv(socket_number, received_message, RECEIVED_MESSAGE_LENGHT, 0);
		    parse_server_message(received_message);
		    close(socket_number); 


		} else if(command == '3') {
			printf("Escreva o nome da disciplina:\n");
			scanf(" %s", discipline);
			socket_number = connectToTheServer();
			message = make_client_message(3, discipline, NULL);
			send(socket_number, message, strlen(message), 0); 
		    recv(socket_number, received_message, RECEIVED_MESSAGE_LENGHT, 0);
		    parse_server_message(received_message);
		    close(socket_number); 

		} else if(command == '4') {
			socket_number = connectToTheServer();
			message = make_client_message(4, NULL, NULL);
			send(socket_number, message, strlen(message), 0); 
		    recv(socket_number, received_message, RECEIVED_MESSAGE_LENGHT, 0);
		    parse_server_message(received_message);
		    close(socket_number); 

		} else if(command == '5') {

			printf("Escreva o nome da disciplina:\n");
			for(int i = 0; i < RECEIVED_MESSAGE_LENGHT; i++) {
    			comment[i] = '\0';
   			}
			scanf(" %s", discipline);
			printf("Escreva o comentário:\n");
			scanf(" %[^\n]", comment);
			socket_number = connectToTheServer();
			message = make_client_message(5, discipline, comment);
			send(socket_number, message, strlen(message), 0); 
		    recv(socket_number, received_message, RECEIVED_MESSAGE_LENGHT, 0);
		    parse_server_message(received_message);
		    close(socket_number); 

		} else if(command == '6') {

			printf("Escreva o nome da disciplina:\n");
			scanf(" %s", discipline);
			socket_number = connectToTheServer();
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

int logInStudent() {

	char * message = make_client_message(0, "MC102", "Eu me amo");

	printf("%s\n", message);
	return OK;
}

int logInProfessor() {
	printf("Conectando como professor...\n");
	return ERROR;
}