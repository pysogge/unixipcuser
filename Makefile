# Make for Unix C 1

.PHONY: test messages messageserver sharedmem

# define the C compiler to use
CC:=gcc

# define any compile-time flags
CFLAGS:=-Wall -g

test:
	gcc ${CFLAGS} -o test.out ./src/test.c

messages:
	gcc ${CFLAGS} -o msgsend.out ./src/msgsend.c
	gcc ${CFLAGS} -o msgrcv.out ./src/msgrcv.c

messageserver:
	gcc ${CFLAGS} -o msgserver.out ./src/msgserver.c
	gcc ${CFLAGS} -o msgclient.out ./src/msgclient.c

sharedmem:
	gcc ${CFLAGS} -o sharedmem_a.out ./src/sharedmem_a.c
	gcc ${CFLAGS} -o sharedmem_b.out ./src/sharedmem_b.c

clean:
	rm -rf ./*.dSYM
	rm -rf ./*.out