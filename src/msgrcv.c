#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#define MSGKEY 1337
#define MSGTYPE 6  // arbitrary

struct msgform {
    long mtype;
    char mtext[256];
};

void printmsg(struct msgform *, int);

int main() {
    struct msgform msg;
    int msgqid, pid;
    int *pintword;  // pointer to a word (integer)

    pid = getpid();

    msg.mtype = MSGTYPE;  // set message struct type to MSGTYPE (arbitrary)

    for (;;) {
        // get message queue
        if ((msgqid = msgget(MSGKEY, 0777)) == -1) {
            perror("msgget: cannot find message que for given key");
            exit(1);
        }
        // receive message
        if (msgrcv(msgqid, &msg, sizeof(int), MSGTYPE, 0) == -1) {
            perror("msgrcv: cannot receive message");
            exit(1);
        }
        // print message
        printmsg(&msg, msgqid);
    }
}

void printmsg(struct msgform *msg, int msgqid) {
    printf("\n receiving process pid: [d]: %d\n", getpid());

    printf("\nmessage received.\n\n");

    printf("received message: \n");
    printf("msgqid [d]: %d\n", msgqid);
    printf("first word of mtext [d]: %d\n", (int)*((int *)msg->mtext));
    printf("msg.mtype [ld]: %ld\n", msg->mtype);
    printf("message.mtext (garbage) [s]: %s\n", msg->mtext);  // garbage since we're reading an int directly from msg address 0
}