#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"


int initialize(char* file_name, FILE* fp, cJSON* json) {
	int size;
	char* fcontent;

	/*abre arquivo*/
	fp = fopen(nome_arq, "r+");
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	fcontent = malloc(size);
	fread(fcontent, 1, size, fp);

	/* inicializa estrutura do BD (json) */
	json = cJSON_Parse(fcontent);
	if(json == NULL)
		return 1;
	return 0;
}

char* make_server_message(int mes_numb, cJSON* content_root) {
	char* message;
	cJSON* json_message;

	json_message = cJSON_CreateObject();
	cJSON_AddNumberToObject(json_message, "message_number", mes_numb);
	cJSON_AddItemToObject(json_message, "content", content_root);
	message = cJSON_Print(content_root);
	return message;
}

char* make_client_message(int mes_numb, char* code, char* comment) {
	char* message;
	cJSON* json_message, content;

	json_message = cJSON_CreateObject();
	cJSON_AddNumberToObject(json_message, "message_number", mes_numb);

	if(code == NULL)
		cJSON_AddNullToObject(json_message, "content");
	else if(comment == NULL) {
		content = cJSON_CreateObject();
		cJSON_AddStringToObject(content, "codigo", code);
		cJSON_AddItemToObject(json_message, "content", content);
	}
	else {
		content = cJSON_CreateObject();
		cJSON_AddStringToObject(content, "codigo", code);
		cJSON_AddStringToObject(content, "commentario", comment);
		cJSON_AddItemToObject(json_message, "content", content);
	}

	message = cJSON_Print(json_message);
	return message;
}

cJSON* fetch_info(int mes_numb, cJSON* content_root) {
	switch (mes_numb) {
		case 1:
		case 2:
		case 3:
	}
}

int parse_server_message(char* message) {
	cJSON* json = NULL;

	json = cJSON_Parse(message);
	return 0;
}

int parse_client_message(char* message, char* nxt_message) {
	cJSON* json = NULL;

	json = cJSON_Parse(message);
	return 0;
}
