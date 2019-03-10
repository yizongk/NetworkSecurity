INSTALLDIR := ./
CC         := /usr/bin/g++
CFLAGS     += -Wall
CLI_OBJECTS:= test_client_protocol_main.o endpoint.o socket.o
SER_OBJECTS:= test_server_protocol_main.o endpoint.o socket.o
EXE        := test_server_protocol_main test_client_protocol_main


.PHONY: all
all: $(EXE)


test_server_protocol_main: $(SER_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	@ echo Changed files: $?

test_client_protocol_main: $(CLI_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	@ echo Changed files: $?


test_server_protocol_main.o: test_server_protocol_main.cpp
	g++ -c test_server_protocol_main.cpp
test_client_protocol_main.o: test_client_protocol_main.cpp
	g++ -c test_client_protocol_main.cpp
socket.o: ../../../src/backend/socket.cpp
	g++ -c ../../../src/backend/socket.cpp
endpoint.o: ../../../src/backend/endpoint.cpp
	g++ -c ../../../src/backend/endpoint.cpp


.PHONY: clean
clean:
	$(RM) $(OBJECTS) 

.PHONE: cleanall
cleanall:
	$(RM) $(OBJECTS) $(EXE)
