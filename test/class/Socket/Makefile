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
socket.o: ../../../src/socket.cpp
	g++ -c ../../../src/socket.cpp


.PHONY: clean
clean:
	$(RM) $(OBJECTS) 

.PHONE: cleanall
cleanall:
	$(RM) $(OBJECTS) $(EXE)
