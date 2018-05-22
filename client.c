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
			sendto(socket_number, message, strlen(message), 0, servinfo->ai_addr, servinfo->ai_addrlen);
			addr_len = sizeof servinfo;
			recvfrom(socket_number, received_message, RECEIVED_MESSAGE_LENGHT-1 , 0, (struct sockaddr_in *) servinfo->ai_addr, &(servinfo->ai_addrlen));
	
		    parse_server_message(received_message);

		} else if(command == '2') {

			printf("Escreva o nome da disciplina:\n");
			scanf(" %s", discipline);
			message = make_client_message(2, discipline, NULL);
			sendto(socket_number, message, strlen(message), 0, servinfo->ai_addr, servinfo->ai_addrlen);
			addr_len = sizeof servinfo;
			recvfrom(socket_number, received_message, RECEIVED_MESSAGE_LENGHT-1 , 0, (struct sockaddr_in *) servinfo->ai_addr, &(servinfo->ai_addrlen));
	
		    parse_server_message(received_message);


		} else if(command == '3') {
			printf("Escreva o nome da disciplina:\n");
			scanf(" %s", discipline);
			
			message = make_client_message(3, discipline, NULL);
			sendto(socket_number, message, strlen(message), 0, servinfo->ai_addr, servinfo->ai_addrlen);
			addr_len = sizeof servinfo;
			recvfrom(socket_number, received_message, RECEIVED_MESSAGE_LENGHT-1 , 0, (struct sockaddr_in *) servinfo->ai_addr, &(servinfo->ai_addrlen));
	
		    parse_server_message(received_message);

		} else if(command == '4') {
			
			message = make_client_message(4, NULL, NULL);
			sendto(socket_number, message, strlen(message), 0, servinfo->ai_addr, servinfo->ai_addrlen);
			addr_len = sizeof servinfo;
			recvfrom(socket_number, received_message, RECEIVED_MESSAGE_LENGHT-1 , 0, (struct sockaddr_in *) servinfo->ai_addr, &(servinfo->ai_addrlen));
	
		    parse_server_message(received_message);

		} else if(command == '5') {

			printf("Escreva o nome da disciplina:\n");
			scanf(" %s", discipline);
			
			message = make_client_message(6, discipline, NULL);
			sendto(socket_number, message, strlen(message), 0, servinfo->ai_addr, servinfo->ai_addrlen);
			addr_len = sizeof servinfo;
			recvfrom(socket_number, received_message, RECEIVED_MESSAGE_LENGHT-1 , 0, (struct sockaddr_in *) servinfo->ai_addr, &(servinfo->ai_addrlen));
	
		    parse_server_message(received_message);

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


	int i;

	struct addrinfo hints;
	struct addrinfo *servinfo;
	int status;
	int socket_number;
	int addr_len;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;  
	hints.ai_socktype = SOCK_DGRAM;   

	status = getaddrinfo(IPServer, "3490", &hints, &servinfo);

    socket_number = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

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

			for(int h = 0; h < 60; h++) {
				message = make_client_message(1, NULL, NULL);
				clock_gettime(CLOCK_REALTIME, &ts1);
				sendto(socket_number, message, strlen(message), 0, servinfo->ai_addr, servinfo->ai_addrlen);
				//timespec_get(&ts1, TIME_UTC);
				
				addr_len = sizeof servinfo;
				recvfrom(socket_number, received_message, RECEIVED_MESSAGE_LENGHT-1 , 0, (struct sockaddr_in *) servinfo->ai_addr, &(servinfo->ai_addrlen));
				clock_gettime(CLOCK_REALTIME, &ts2);

			    //parse_server_message(received_message);

			    double wtf = ts2.tv_nsec - ts1.tv_nsec;
				printf("%lf\n", wtf);
			}
			
			

		} else if(command == '2') {

			printf("Escreva o nome da disciplina:\n");
			scanf(" %s", discipline);
			
			message = make_client_message(2, discipline, NULL);
			sendto(socket_number, message, strlen(message), 0, servinfo->ai_addr, servinfo->ai_addrlen);
			addr_len = sizeof servinfo;
			recvfrom(socket_number, received_message, RECEIVED_MESSAGE_LENGHT-1 , 0, (struct sockaddr_in *) servinfo->ai_addr, &(servinfo->ai_addrlen));
	
		    parse_server_message(received_message);


		} else if(command == '3') {
			printf("Escreva o nome da disciplina:\n");
			scanf(" %s", discipline);
			
			message = make_client_message(3, discipline, NULL);
			sendto(socket_number, message, strlen(message), 0, servinfo->ai_addr, servinfo->ai_addrlen);
			addr_len = sizeof servinfo;
			recvfrom(socket_number, received_message, RECEIVED_MESSAGE_LENGHT-1 , 0, (struct sockaddr_in *) servinfo->ai_addr, &(servinfo->ai_addrlen));
	
		    parse_server_message(received_message);

		} else if(command == '4') {
			
			message = make_client_message(4, NULL, NULL);
			sendto(socket_number, message, strlen(message), 0, servinfo->ai_addr, servinfo->ai_addrlen);
			addr_len = sizeof servinfo;
			recvfrom(socket_number, received_message, RECEIVED_MESSAGE_LENGHT-1 , 0, (struct sockaddr_in *) servinfo->ai_addr, &(servinfo->ai_addrlen));
	
		    parse_server_message(received_message);

		} else if(command == '5') {

			printf("Escreva o nome da disciplina:\n");
			for(i = 0; i < RECEIVED_MESSAGE_LENGHT; i++) {
    			comment[i] = '\0';
 			}
			scanf(" %s", discipline);
			printf("Escreva o comentário:\n");
			scanf(" %[^\n]", comment);
			printf("comentario: %s\n", comment);
			
			message = make_client_message(5, discipline, comment);
			sendto(socket_number, message, strlen(message), 0, servinfo->ai_addr, servinfo->ai_addrlen);
			addr_len = sizeof servinfo;
			recvfrom(socket_number, received_message, RECEIVED_MESSAGE_LENGHT-1 , 0, (struct sockaddr_in *) servinfo->ai_addr, &(servinfo->ai_addrlen));
	
		    parse_server_message(received_message);
		    printf("Mensagem escrita com sucesso!\n");
		    

		} else if(command == '6') {

			printf("Escreva o nome da disciplina:\n");
			scanf(" %s", discipline);
			
			message = make_client_message(6, discipline, NULL);
			sendto(socket_number, message, strlen(message), 0, servinfo->ai_addr, servinfo->ai_addrlen);
			addr_len = sizeof servinfo;
			recvfrom(socket_number, received_message, RECEIVED_MESSAGE_LENGHT-1 , 0, (struct sockaddr_in *) servinfo->ai_addr, &(servinfo->ai_addrlen));
	
		    parse_server_message(received_message);
		}else {
			printf("Ops! Este não é um número válido!\n");
		}


		

	}

	while(logInStudent() == ERROR) {
		printf("Erro de conexão! Tentando novamente...\n");
	}
}
