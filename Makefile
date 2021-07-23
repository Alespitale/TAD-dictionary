CC=gcc
CFLAGS=-Wall -Werror -Wextra -pedantic -std=c99 -g

all: main.o string.o dict.o dict_helpers.o helpers.o
		$(CC) $(CFLAGS) -o dict main.o string.o dict.o dict_helpers.o helpers.o

main.o: main.c string.h dict.h dict_helpers.h helpers.h
		$(CC) $(CFLAGS) -c main.c

string.o: string.h
		$(CC) $(CFLAGS) -c string.c

dict.o: dict.h string.h
		$(CC) $(CFLAGS) -c dict.c

dict_helpers.o: dict.h string.h helpers.h
		$(CC) $(CFLAGS) -c dict_helpers.c

helpers.o: helpers.h
		$(CC) $(CFLAGS) -c helpers.c

.PHONY: clean
clean:
		rm -f all main.o helpers.o string.o dict_helpers.o dict.o dict
