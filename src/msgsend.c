// run with -r flag to reset the message queue
// run with -k flag to delete the message queue (and not send message)

#include <inttypes.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MSGKEY 1337
#define MSGTYPE 6  // arbitrary

struct msgform {
    long mtype;
    char mtext[256];
};

int main(int argc, char *argv[]) {
    struct msgform msg;
    int msgqid, pid;
    int *pintword; // pointer to a word (integer)

    int reset = 0;
    if (argc > 1) {
        if (strcmp(argv[1], "-r") == 0) {
            reset = 1;
        }
        if(strcmp(argv[1], "-k") == 0) {
            msgqid = msgget(MSGKEY, 0777 | IPC_CREAT);
            msgctl(msgqid, IPC_RMID, 0);
            exit(0);
        }
    }

    pid = getpid();

    msg.mtype = MSGTYPE;  // set message struct type to MSGTYPE (arbitrary)
    pintword = (int *)msg.mtext; // set first word addr of message to ptr
    *pintword = pid;      // setting first word of msg to sender's pid 



    // remove any previous message queue with key MSGKEY
    if(reset == 1 && (msgqid = msgget(MSGKEY, 0777)) != -1) {
        msgctl(msgqid, IPC_RMID, 0);
    }

    msgqid = msgget(MSGKEY, 0777 | IPC_CREAT); // get (create new) message queue

    msgsnd(msgqid, &msg, sizeof(int), 0);  // send message
    // only sending the first word of the message, not the whole char[256]

    printf("\nthis processes pid: [d]: %d\n", *(pintword));

    printf("\nmessage sent.\n\n");

    printf("sent message: \n");
    printf("msgqid [d]: %d\n", msgqid);
    printf("first word of mtext [d]: %d\n", (int)*((int *)msg.mtext));
    printf("msg.mtype [ld]: %ld\n", msg.mtype);
    printf("message.mtext (garbage) [s]: %s\n", msg.mtext); // garbage since we're writing an int directly to address 0
}