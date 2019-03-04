INSTALLDIR := ./
CC         := /usr/bin/g++
CFLAGS     += -Wall
OBJECTS    := test_server_client_comm_main.o server.o client.o socket.o
EXE        := test_server_client_comm_main


.PHONY: all
all: $(EXE)


test_server_client_main: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	@ echo Changed files: $?

test_server_client_comm_main.o: test_server_client_comm_main.cpp
	g++ -c test_server_client_comm_main.cpp
socket.o: ../../../src/backend/socket.cpp
	g++ -c ../../../src/backend/socket.cpp
server.o: ../../../src/backend/server.cpp
	g++ -c ../../../src/backend/server.cpp
client.o: ../../../src/backend/client.cpp
	g++ -c ../../../src/backend/client.cpp


.PHONY: clean
clean:
	$(RM) $(OBJECTS) 

.PHONE: cleanall
cleanall:
	$(RM) $(OBJECTS) $(EXE)
