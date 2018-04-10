#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"


int initialize(char* file_name, FILE* fp, cJSON* json) {
	int size;
	char* fcontent;

	/*open file*/
	fp = fopen(nome_arq, "r+");
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	fcontent = malloc(size);
	fread(fcontent, 1, size, fp);

	/* initialize db structure (json) */
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

cJSON* fetch_info(int mes_numb, cJSON* db, char* code) {
	cJSON* it, arr, info_obj, info_arr, disc_obj, elem;
	char* cod, aux;

	switch (mes_numb) {
		case 1:
			arr = cJSON_GetObjectItemCaseSensitive(db, "disciplinas");
			info_obj = cJSON_CreateObject();
			info_arr = cJSON_CreateArray();
			/*iterating over objects of array disciplinas*/
			cJSON_ArrayForEach(it, arr -> child) {
				/*creating the object containing only the asked infos*/
				disc_obj = cJSON_CreateObject();
				elem = cJSON_GetObjectItemCaseSensitive(it, "codigo") -> valuestring;
				aux = malloc(sizeof(elem));
				strcpy(aux, elem);
				cJSON_AddStringToObject(disc_obj, "codigo",	aux);
				elem = cJSON_GetObjectItemCaseSensitive(it, "nome") -> valuestring;
				aux = malloc(sizeof(elem));
				strcpy(aux, elem);
				cJSON_AddStringToObject(disc_obj, "nome",	aux);

				cJSON_AddItemToArray(info_arr, disc_obj);
			}
			cJSON_AddItemToObject(info_obj, "disciplinas", info_arr);
			break;
		case 2:
			arr = cJSON_GetObjectItemCaseSensitive(db, "disciplinas");
			info_obj = cJSON_CreateObject();
			info_arr = cJSON_CreateArray();
			cJSON_ArrayForEach(it, arr -> child) {
				cod = cJSON_GetObjectItemCaseSensitive(it, "codigo") -> valuestring;
				if(strcmp(cod, code) == 0) {
					disc_obj = cJSON_CreateObject();
					elem = cJSON_GetObjectItemCaseSensitive(it, "nome") -> valuestring);
					aux = malloc(sizeof(elem));
					strcpy(aux, elem);
					cJSON_AddStringToObject(disc_obj, "nome",	aux);

					cJSON_AddItemToArray(info_arr, disc_obj);
					break;
				}
			}
			cJSON_AddItemToObject(info_obj, "disciplinas", info_arr);
			break;
		case 3:
			arr = cJSON_GetObjectItemCaseSensitive(db, "disciplinas");
			cJSON_ArrayForEach(it, arr -> child) {
				cod = cJSON_GetObjectItemCaseSensitive(it, "codigo") -> valuestring;
				if(strcmp(cod, code) == 0) {
					info_arr = cJSON_CreateArrayReference(cJSON_CreateObjectReference(it -> child));
					aux = malloc(11 * sizeof(char))
					aux = "disciplinas";
					info_obj = cJSON_CreateObjectReference(info_arr);

					break;
				}
			}
			break;
		case 4:
			info_obj = cJSON_CreateObjectReference(db -> child);
			break;
		case 6:
			arr = cJSON_GetObjectItemCaseSensitive(db, "disciplinas");
			info_obj = cJSON_CreateObject();
			info_arr = cJSON_CreateArray();
			cJSON_ArrayForEach(it, arr -> child) {
				cod = cJSON_GetObjectItemCaseSensitive(it, "codigo") -> valuestring;
				if(strcmp(cod, code) == 0) {
					disc_obj = cJSON_CreateObject();
					elem = cJSON_GetObjectItemCaseSensitive(it, "comentario") -> valuestring);
					aux = malloc(sizeof(elem));
					strcpy(aux, elem);
					cJSON_AddStringToObject(disc_obj, "nome",	aux);

					cJSON_AddItemToArray(info_arr, disc_obj);
					break;
				}
			}
			cJSON_AddItemToObject(info_obj, "disciplinas", info_arr);
			break;
	}

	return info_obj;
}

void write_comment(cJSON* db, char* code, char* comment) {
	cJSON* arr, it, comment_str_obj, aux;

	arr = cJSON_GetObjectItemCaseSensitive(db, "disciplinas");
	cJSON_ArrayForEach(it, arr -> child) {
		cod = cJSON_GetObjectItemCaseSensitive(it, "codigo") -> valuestring;
		if(strcmp(cod, code) == 0) {
			aux = malloc(sizeof(comment));
			strcpy(aux, comment);
			comment_str_obj = cJSON_CreateString(aux);
			cJSON_ReplaceItemInObjectCaseSensitive(it, "comentario", comment_str_obj);
			break;
		}
	}
}

int parse_server_message(char* message) {
	cJSON* json = NULL;

	json = cJSON_Parse(message);
	return 0;
}

int parse_client_message(char* message, char* nxt_message, cJSON* db) {
	int mes_number;
	char* codigo, comentario;
	cJSON* json, info, mes_content;

	json = cJSON_Parse(message);
	mes_numb = cJSON_GetObjectItemCaseSensitive(json, "message_number") -> valueint;
	if(mes_numb == 3 && mes_numb == 4) {
		/*need to fetch some information in the db*/
		info = fetch_info(mes_numb, db, NULL);
		nxt_message = make_server_message(mes_numb, info);
	}
	else {
		mes_content = cJSON_GetObjectItemCaseSensitive(json, "content");
		codigo = cJSON_GetObjectItemCaseSensitive(mes_content, "codigo");
		if (mes_numb == 5){
			comentario = cJSON_GetObjectItemCaseSensitive(mes_content, "comentario");
			write_comment(db, codigo, comentario);
		}
		else {
			info = fetch_info(mes_numb, db, codigo);
			nxt_message = make_server_message(mes_numb, info);
		}
	}

	return 0;
}