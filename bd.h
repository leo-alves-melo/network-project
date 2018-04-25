/*data base utilities*/

/* ------------------ README --------------------------
   you MUST only use the following functions:
   - initialize
   - make_client_message
   - parse_client_message
   - parse_server_message

   the remaining function should not be called because they are internally
   by the exposed functions. */

#include "cJSON.h"

/* initialize the data structure (json)
   given a file name, it returns the file pointer and the json tree structure
   also return an integer indicating error */
int initialize(char* file_name, FILE** fp, cJSON** json);

/* fetch some data in the json structure stored in the db. the data that is
   going to be fetched depends on the type of message that the server will send.
   given the message number, the db and maybe a course code, it returns a json
   structure with an object as root similar to the default structure of the bd. */
cJSON* fetch_info(int mes_numb, cJSON* db, char* code);

/*Replace the class comment.  */
void write_comment(cJSON* db, char* code, char* comment);

/* make a server message with the message number and the content fetched in
   the data base.
   given the message number and the content to be sent, returns a string
   representation of the message. */
char* make_server_message(int mes_numb, cJSON* db);

/* make a client message with the message number and content. the latter is an
   object that can have course code and/or class comment.
   given the message number, the code and the comment, it returns the string
   representation of the message
   note: this function behavior is determined by its message number, so for
   example if a message has number 1, it doesn't require neither code nor comment.
   it means that it is possible to put anything in these params and the function
   will still works. but to avoid problems it is highly recommend to put NULLs
   in these fields if will not use them. */
char* make_client_message(int mes_numb, char* code, char* comment);

/* parse a server message received by the client and prints in the screen
   the data requested.
   given the message , it returns an integer indicating if the function
   worked properly */
int parse_server_message(char* message);

/* parse a client message received by the server and returns a message that
   contains the requested data that will be sent back to the client.
   given the message and the db, it returns an integer indicating if the
   function worked properly and a pointer to the next message  */
int parse_client_message(char* message, char** nxt_message, cJSON* db);
