INSTALLDIR := ./
CC         := /usr/bin/g++
CFLAGS     += -Wall
OBJECTS    := test_client_main.o client.o socket.o
EXE        := test_client_main


.PHONY: all
all: $(EXE)


test_client_main: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	@ echo Changed files: $?

test_client_main.o: test_client_main.cpp
	g++ -c test_client_main.cpp
socket.o: ../../../src/backend/socket.cpp
	g++ -c ../../../src/backend/socket.cpp
client.o: ../../../src/backend/client.cpp
	g++ -c ../../../src/backend/client.cpp


.PHONY: clean
clean:
	$(RM) $(OBJECTS) 

.PHONE: cleanall
cleanall:
	$(RM) $(OBJECTS) $(EXE)
