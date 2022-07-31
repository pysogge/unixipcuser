#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define SHMKEY 1337
#define KBYTES 1024
#define NUMBYTES 256
#define SHMACCESS 0777

#define NULLPTR 0

int shmid;
int* virtaddr;

int main() {
    int i, pid;
    int* pintword;

    extern void* shmat(int, const void*, int);
    extern void cleanup();

    for (i = 0; i < 20; i++) {
        signal(i, cleanup);
    }

    // create a new shared memory segment (shmem table)
    // shmget(1337, 256 * 1024, 0777);
    shmid = shmget(SHMKEY, NUMBYTES * KBYTES, SHMACCESS | IPC_CREAT);
    // attach the shared memory segment to the process's address space
    // (allocates memory, and returns a pointer to the shared memory segment)

    virtaddr = (int*)shmat(shmid, NULLPTR, 0);

    pintword = (int*)virtaddr;

    pid = getpid();
    printf("Process [%d] writing to shared memory...\n", pid);

    // write to the shared memory segment - [0]: pid, then count 1 to 256
    *pintword++ = pid;
    for (i = 1; i < 256; i++) {
        *pintword++ = i;
    }
    pintword = (int*)virtaddr;

    printf("Process [%d] wrote to shared memory SHMKEY: %d; shmid: %d, addr: %p \n", pid, SHMKEY, shmid, virtaddr);

    // wait for signal to clear shared memory
    printf("Awaiting process termination signal...\n");
    pause();
}

void cleanup() {
    int pid = getpid();
    int killedbypid = *virtaddr;
    // shmdt(addr);
    shmctl(shmid, IPC_RMID, 0);
    printf("Process [%d] terminated by pid %d shared memory shmid: %d\n", pid, killedbypid, shmid);
    exit(0);
}