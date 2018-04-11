HEADERS = cJSON.h bd.h
OBJECTS = cJSON.o bd.o teste.o

default: teste

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

teste: $(OBJECTS)
	gcc $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f program
