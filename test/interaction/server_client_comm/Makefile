INSTALLDIR := ./
CC         := /usr/bin/g++
CFLAGS     += -Wall
OBJECTS    := test_server_client_comm_main.o endpoint.o socket.o shinyarmor_hdr.o
CLI_OBJECTS:= test_client_main.o endpoint.o socket.o shinyarmor_hdr.o
SER_OBJECTS:= test_server_main.o endpoint.o socket.o shinyarmor_hdr.o
EXE        := test_server_client_comm_main test_server_main test_client_main


.PHONY: all
all: $(EXE)


test_server_client_comm_main: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	@ echo Changed files: $?

test_server_main: $(SER_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	@ echo Changed files: $?

test_client_main: $(CLI_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	@ echo Changed files: $?

test_server_client_comm_main.o: test_server_client_comm_main.cpp
	g++ -c test_server_client_comm_main.cpp
test_server_main.o: test_server_main.cpp
	g++ -c test_server_main.cpp
test_client_main.o: test_client_main.cpp
	g++ -c test_client_main.cpp
socket.o: ../../../src/backend/socket.cpp
	g++ -c ../../../src/backend/socket.cpp
endpoint.o: ../../../src/backend/endpoint.cpp
	g++ -c ../../../src/backend/endpoint.cpp
shinyarmor_hdr.o:
	g++ -c ../../../src/backend/shinyarmor_hdr.cpp



.PHONY: clean
clean:
	$(RM) $(OBJECTS) 

.PHONE: cleanall
cleanall:
	$(RM) $(OBJECTS) $(EXE) $(CLI_OBJECTS) $(SER_OBJECTS)
