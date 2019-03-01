INSTALLDIR := ./
CC         := /usr/bin/g++
CFLAGS     += -Wall
OBJECTS    := main.o socket.o
EXE        := test


.PHONY: all
all: $(EXE)


test: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	@ echo Changed files: $?

main.o: main.cpp
	g++ -c main.cpp
socket.o: ../../../src/backend/socket.cpp
	g++ -c ../../../src/backend/socket.cpp


.PHONY: clean
clean:
	$(RM) $(OBJECTS) 

.PHONE: cleanall
cleanall:
	$(RM) $(OBJECTS) $(EXE)
