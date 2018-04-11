/*data base utilits*/

/* initialize the data structure (json)
   given a file name, it returns the file pointer and the json tree structure
   also return an integer indicating error */
int initialize(char* file_name, FILE** fp, cJSON** json);

/* fetch some data in the json structure stored in the db. the data that is
   going to be fetched depends on the type of message that the server will send.
   given the message number and the db, returns a json structure with an object
   as root similar to the default structure of the bd.  */
cJSON* fetch_info(int mes_numb, cJSON* db, char* code);

void write_comment(cJSON* db, char* code, char* comment);

/* make a server message with the message number and the content fetched in
   the data base.
   given the message number and the content to be sent, returns a string
   representation of the message. */
char* make_server_message(int mes_numb, cJSON* db);

/* make a client message with the message number and content, that can be
   a code and/or a comment
   given the messaage number, the code and the comment, it returns the string
   representation of the message */
char* make_client_message(int mes_numb, char* code, char* comment);

/* parse a server message received by the client and prints in the screen
   the data requested.
   given the message number, it returns an integer indicating if the function
   worked properly */
int parse_server_message(char* message);

/* parse a client message received by the server and returns a message that is
   contains the requested data
   given the message number and a pointer to the message that will be sent,
   it returns an integer indicating if the function worked properly  */
int parse_client_message(char* message, char** nxt_message, cJSON* db);
