# Make for Unix C 1

.PHONY: test

# define the C compiler to use
CC:=gcc

# define any compile-time flags
CFLAGS:=-Wall -g

test:
	gcc -o test ./src/test.c

messages:
	gcc -o msgsend ./src/msgsend.c
	gcc -o msgrcv ./src/msgrcv.c

messageserver:
	gcc -o msgserver ./src/msgserver.c
	gcc -o msgclient ./src/msgclient.c