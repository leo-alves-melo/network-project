HEADERS = cJSON.h bd.h
OBJECTS_SERVER = cJSON.o bd.o server.o
OBJECTS_CLIENT = cJSON.o bd.o client.o

default: server client

%.o: %.c $(HEADERS)
	gcc -pthread -c $< -o $@

server: $(OBJECTS_SERVER)
	gcc -pthread $(OBJECTS_SERVER) -o $@

client: $(OBJECTS_CLIENT)
	gcc -pthread $(OBJECTS_CLIENT) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f program
