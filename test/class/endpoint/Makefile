INSTALLDIR := ./
CC         := /usr/bin/g++
CFLAGS     += -Wall -pthread
OBJECTS    := test_endpoint_main.o endpoint.o socket.o
EXE        := test_endpoint_main


.PHONY: all
all: $(EXE)


test_endpoint_main: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	@ echo Changed files: $?

test_endpoint_main.o: test_endpoint_main.cpp
	g++ -c test_endpoint_main.cpp
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
