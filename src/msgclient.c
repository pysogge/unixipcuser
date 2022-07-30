// run with -r flag to reset the message queue
// run with -k flag to delete the message queue (and not send message)

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#define MSGKEY 1338
#define MSGTYPE 1  // arbitrary (but matches server's MSGTYPE)
#define WAITRESPONSETIMEOUT 5

struct msgform {
    long mtype;
    char mtext[256];
};

int main(int argc, char *argv[]) {
    struct msgform msg;
    int msgqid, pid, spid; // pid is this process's pid, spid is server's pid
    int *pintword;  // pointer to a word (integer)

    pid = getpid();

    msg.mtype = MSGTYPE;          // set message struct type to MSGTYPE (arbitrary)
    pintword = (int *)msg.mtext;  // set first word addr of message to ptr
    *pintword = pid;              // setting first word of msg to sender's pid

    // remove any previous message queue with key MSGKEY
    if ((msgqid = msgget(MSGKEY, 0777)) == -1) {
        perror("msgget: no Message Queue with MSGKEY found");
        exit(0);
        // msgctl(msgqid, IPC_RMID, 0);
    }

    msgsnd(msgqid, &msg, sizeof(int), 0);  // send message (this pid)

    printf("\nthis processes pid: [d]: %d\n", *(pintword));

    printf("\nmessage sent.\n\n");

    printf("sent message: \n");
    printf("msgqid [d]: %d\n", msgqid);
    printf("first word of mtext [d]: %d\n", (int)*((int *)msg.mtext));
    printf("msg.mtype [ld]: %ld\n", msg.mtype);
    printf("message.mtext (garbage) [s]: %s\n", msg.mtext);  // garbage since we're writing an int directly to address 0

    // await server response
    for (int i = 0; i < (int)WAITRESPONSETIMEOUT; i++) {
        // await a message response from server,
        if (msgrcv(msgqid, &msg, sizeof(int), pid, 0) > 0) {
            pintword = (int *)msg.mtext;  // set first word addr of message to ptr
            spid = *pintword;             // storing message as client pid
            printf("\nreceived response from server pid: [d]: %d\n", spid);
            printf("received response from server mtype: [d]: %ld\n", msg.mtype);
            exit(0);
        }
        usleep(100000);
    }
    perror("\nserver response timeout\n");
    exit(1);
}