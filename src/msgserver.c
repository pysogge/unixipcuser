#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#define MSGKEY 1338
#define MSGTYPE 1  // arbitrary

struct msgform {
    long mtype;
    char mtext[256];
} msg;

int msgqid;

void cleanup();

int main() {
    int i, spid, cpid;  // i is a counter, spid is server pid, cpid is client pid
    int *pintword;      // pointer to a word (integer)

    for (i = 0; i < 20; i++)
        signal(i, cleanup);

    msgqid = msgget(MSGKEY, 0777 | IPC_CREAT);  // get (create new) message queue

    spid = getpid();  // get server pid

    printf("process: %d; awaiting a client message...\n", spid);

    for (;;) {
        // receive message (first word of mtext)
        if (msgrcv(msgqid, &msg, sizeof(int), MSGTYPE, 0) < 0) {
            continue;
        }
        pintword = (int *)msg.mtext;  // set first word addr of message to ptr
        cpid = *pintword;             // storing message as client pid
        printf("\nreceived client pid: [d]: %d\n", cpid);

        // send message (send server pid message, and client pid as type)

        *pintword = spid;  // updating the first word of the msg.mtext
        msg.mtype = cpid;  // setting the message type to the client pid

        msgsnd(msgqid, &msg, sizeof(int), 0);  // send message
    }
}

void cleanup() {
    msgctl(msgqid, IPC_RMID, 0);
    exit(0);
}