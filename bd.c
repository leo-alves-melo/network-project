#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "bd.h"

int initialize(char* file_name, FILE** fp, cJSON** json) {
	int size;
	char *fcontent;

	/*open file*/
	*fp = fopen(file_name, "r+");
	fseek(*fp, 0, SEEK_END);
	size = ftell(*fp);
	fseek(*fp, 0, SEEK_SET);
	fcontent = malloc(size);
	fread(fcontent, 1, size, *fp);

	/* initialize db structure (json) */
	*json = cJSON_Parse(fcontent);
	if(*json == NULL)
		return 1;
	return 0;
}

char* make_server_message(int mes_numb, cJSON* content_root) {
	char *message;
	cJSON *json_message;

	json_message = cJSON_CreateObject();
	cJSON_AddNumberToObject(json_message, "message_number", mes_numb);
	cJSON_AddItemReferenceToObject(json_message, "content", content_root);
	message = cJSON_Print(json_message);
	cJSON_Delete(json_message);
	return message;
}

char* make_client_message(int mes_numb, char* code, char* comment) {
	char *message;
	cJSON *json_message, *content;

	json_message = cJSON_CreateObject();
	cJSON_AddNumberToObject(json_message, "message_number", mes_numb);

	if(code == NULL)
		cJSON_AddNullToObject(json_message, "content");
	else if(comment == NULL) {
		content = cJSON_CreateObject();
		cJSON_AddItemToObject(content, "codigo", cJSON_CreateStringReference(code));
		cJSON_AddItemToObject(json_message, "content", content);
	}
	else {
		content = cJSON_CreateObject();
		cJSON_AddItemToObject(content, "codigo", cJSON_CreateStringReference(code));
		cJSON_AddItemToObject(content, "comentario", cJSON_CreateStringReference(comment));
		cJSON_AddItemToObject(json_message, "content", content);
	}

	message = cJSON_Print(json_message);
	cJSON_Delete(json_message);

	return message;
}

cJSON* fetch_info(int mes_numb, cJSON* db, char* code) {
	cJSON *it, *arr, *info_obj, *info_arr, *disc_obj;
	char *cod, *aux, *elem;

	switch (mes_numb) {
		case 1:
			arr = cJSON_GetObjectItemCaseSensitive(db, "disciplinas");
			info_obj = cJSON_CreateObject();
			info_arr = cJSON_CreateArray();
			/*iterating over objects of array disciplinas*/
			for(it = arr -> child; it != NULL; it = it -> next) {
				/*creating the object containing only the asked infos*/
				disc_obj = cJSON_CreateObject();
				elem = cJSON_GetObjectItemCaseSensitive(it, "nome") -> valuestring;
				cJSON_AddItemToObject(disc_obj, "nome", cJSON_CreateStringReference(elem));
				elem = cJSON_GetObjectItemCaseSensitive(it, "codigo") -> valuestring;
				cJSON_AddItemToObject(disc_obj, "codigo", cJSON_CreateStringReference(elem));
				cJSON_AddItemToArray(info_arr, disc_obj);
			}

			cJSON_AddItemToObject(info_obj, "disciplinas", info_arr);
			break;
		case 2:
			arr = cJSON_GetObjectItemCaseSensitive(db, "disciplinas");
			info_obj = cJSON_CreateObject();
			info_arr = cJSON_CreateArray();

			for(it = arr -> child; it != NULL; it = it -> next) {
				cod = cJSON_GetObjectItemCaseSensitive(it, "codigo") -> valuestring;
				if(strcmp(cod, code) == 0) {
					disc_obj = cJSON_CreateObject();
					elem = cJSON_GetObjectItemCaseSensitive(it, "ementa") -> valuestring;
					cJSON_AddItemToObject(disc_obj, "ementa", cJSON_CreateStringReference(elem));
					cJSON_AddItemToArray(info_arr, disc_obj);
					break;
				}
			}

			cJSON_AddItemToObject(info_obj, "disciplinas", info_arr);
			break;
		case 3:
			arr = cJSON_GetObjectItemCaseSensitive(db, "disciplinas");
			info_obj = cJSON_CreateObject();
			info_arr = cJSON_CreateArray();

			for(it = arr -> child; it != NULL; it = it -> next) {
				cod = cJSON_GetObjectItemCaseSensitive(it, "codigo") -> valuestring;
				if(strcmp(cod, code) == 0) {
					cJSON_AddItemToArray(info_arr, cJSON_CreateObjectReference(it -> child));
					break;
				}
			}

			cJSON_AddItemToObject(info_obj, "disciplinas", info_arr);
			break;
		case 4:
			info_obj = cJSON_CreateObjectReference(db -> child);
			break;
		case 6:
			arr = cJSON_GetObjectItemCaseSensitive(db, "disciplinas");
			info_obj = cJSON_CreateObject();
			info_arr = cJSON_CreateArray();

			for(it = arr -> child; it != NULL; it = it -> next) {
				cod = cJSON_GetObjectItemCaseSensitive(it, "codigo") -> valuestring;
				if(strcmp(cod, code) == 0) {
					disc_obj = cJSON_CreateObject();
					elem = cJSON_GetObjectItemCaseSensitive(it, "comentario") -> valuestring;
					cJSON_AddItemToObject(disc_obj, "comentario", cJSON_CreateStringReference(elem));
					cJSON_AddItemToArray(info_arr, disc_obj);
					break;
				}
			}

			cJSON_AddItemToObject(info_obj, "disciplinas", info_arr);
			break;
	}

	return info_obj;
}

void write_db_to_file(cJSON* db, char* file_name) {
	FILE *file = fopen(file_name, "w");
	if (file != NULL)
    {

        fputs(cJSON_Print(db), file);
        fclose(file);
    }
}

void write_comment(cJSON* db, char* code, char* comment) {
	cJSON *arr, *it, *comment_str_obj;
	char *cod, *aux;

	arr = cJSON_GetObjectItemCaseSensitive(db, "disciplinas");

	for(it = arr -> child; it != NULL; it = it -> next) {
		cod = cJSON_GetObjectItemCaseSensitive(it, "codigo") -> valuestring;
		if(strcmp(cod, code) == 0) {
			comment_str_obj = cJSON_CreateStringReference(comment);
			cJSON_ReplaceItemInObjectCaseSensitive(it, "comentario", comment_str_obj);
			break;
		}
	}
	write_db_to_file(db, "data.json");
}

int parse_server_message(char* message) {
	cJSON *json, *cont, *obj, *it;

	json = cJSON_Parse(message);
	cont = cJSON_GetObjectItemCaseSensitive(json, "content");
	switch (cJSON_GetObjectItemCaseSensitive(json, "message_number") -> valueint) {
		case 1:
			/*obj is the first item of the only object present in the array*/
			for(it = cont -> child -> child; it != NULL; it = it -> next) {
				printf("Título: %s\n", cJSON_GetObjectItemCaseSensitive(it, "nome") -> valuestring);
				printf("Código: %s\n", cJSON_GetObjectItemCaseSensitive(it, "codigo") -> valuestring);
				printf("\n");
			}
			break;
		case 2:
			obj = cont -> child -> child;
			if(obj == NULL) {
				printf("Não existe uma disciplina com esse nome!\n");
			}
			else {

				printf("Ementa: %s\n", cJSON_GetObjectItemCaseSensitive(obj, "ementa") -> valuestring);
			}

			break;
		case 3:
			obj = cont -> child -> child;

			if(obj == NULL) {
				printf("Não existe uma disciplina com esse nome!\n");
			}
			else {
				printf("Título: %s\n", cJSON_GetObjectItemCaseSensitive(obj, "nome") -> valuestring);
				printf("Código: %s\n", cJSON_GetObjectItemCaseSensitive(obj, "codigo") -> valuestring);
				printf("Ementa: %s\n", cJSON_GetObjectItemCaseSensitive(obj, "ementa") -> valuestring);
				printf("Sala: %s\n", cJSON_GetObjectItemCaseSensitive(obj, "sala") -> valuestring);
				printf("Horário: %s\n", cJSON_GetObjectItemCaseSensitive(obj, "horario") -> valuestring);
				printf("Comentário: %s\n", cJSON_GetObjectItemCaseSensitive(obj, "comentario") -> valuestring);
			}
			break;
		case 4:
			for(it = cont -> child -> child; it != NULL; it = it -> next) {
				printf("Título: %s\n", cJSON_GetObjectItemCaseSensitive(it, "nome") -> valuestring);
				printf("Código: %s\n", cJSON_GetObjectItemCaseSensitive(it, "codigo") -> valuestring);
				printf("Ementa: %s\n", cJSON_GetObjectItemCaseSensitive(it, "ementa") -> valuestring);
				printf("Sala: %s\n", cJSON_GetObjectItemCaseSensitive(it, "sala") -> valuestring);
				printf("Horário: %s\n", cJSON_GetObjectItemCaseSensitive(it, "horario") -> valuestring);
				printf("Comentário: %s\n", cJSON_GetObjectItemCaseSensitive(it, "comentario") -> valuestring);
				printf("\n");
			}
			break;
		case 5:
			printf("O seu comentário foi escrito com sucesso!");
			break;
		case 6:
			obj = cont -> child -> child;
			if(obj == NULL) {
				printf("Não existe uma disciplina com esse nome!\n");
			}
			else {
				printf("Comentário: %s\n", cJSON_GetObjectItemCaseSensitive(obj, "comentario") -> valuestring);
			}
			break;
	}

	return 0;
}

int parse_client_message(char* message, char** nxt_message, cJSON* db) {
	int mes_number;
	char *codigo, *comentario;
	cJSON *json, *info, *mes_content;


	json = cJSON_Parse(message);
	mes_number = cJSON_GetObjectItemCaseSensitive(json, "message_number") -> valueint;
	if(mes_number == 1 || mes_number == 4) {
		/*need to fetch some information in the db*/
		info = fetch_info(mes_number, db, NULL);
		*nxt_message = make_server_message(mes_number, info);
		cJSON_Delete(info);
	}
	else {
		mes_content = cJSON_GetObjectItemCaseSensitive(json, "content");
		codigo = cJSON_GetObjectItemCaseSensitive(mes_content, "codigo") -> valuestring;
		if (mes_number == 5){

			comentario = cJSON_GetObjectItemCaseSensitive(mes_content, "comentario")  -> valuestring;

			write_comment(db, codigo, comentario);
			info = fetch_info(mes_number, db, codigo);
			*nxt_message = make_server_message(mes_number, info);

		}
		else {
			info = fetch_info(mes_number, db, codigo);
			*nxt_message = make_server_message(mes_number, info);
			cJSON_Delete(info);
		}
	}

	return 0;
}
