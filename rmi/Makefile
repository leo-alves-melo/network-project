OBJECTS_SERVER = Server.java
OBJECTS_CLIENT = Client.java
OBJECTS_INTERFACE = Connection.java

default: interface server client

interface: $(OBJECTS_INTERFACE)
	javac $(OBJECTS_INTERFACE)

server: $(OBJECTS_SERVER)
	javac $(OBJECTS_SERVER)

client: $(OBJECTS_CLIENT)
	javac $(OBJECTS_CLIENT)

runServer:
	java -Djava.security.manager -Djava.security.policy=server.policy Server

runClient:
	java -Djava.security.manager -Djava.security.policy=client.policy Client

runRMI:
	rmiregistry