#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>  // sleep
#include <signal.h> // kill

#define SHMKEY 1337
#define KBYTES 1024
#define NUMBYTES 256
#define SHMACCESS 0777

#define NULLPTR 0

int shmid;

int main() {
    int i, pid, killpid;
    int* pintword;
    int* virtaddr;
    extern void* shmat(int, const void*, int);
    extern void cleanup();
    extern int kill(pid_t pid, int sig);

    pid = getpid();

    // create a new shared memory segment (shmem table)
    // shmget(1337, 256 * 1024, 0777);
    shmid = shmget(SHMKEY, NUMBYTES * KBYTES, SHMACCESS | IPC_CREAT);
    // attach the shared memory segment to the process's address space
    // (allocates memory, and returns a pointer to the shared memory segment)
    virtaddr = (int*)shmat(shmid, NULLPTR, 0);

    pintword = (int*)virtaddr;

        printf("Process [%d] reading from shared memory KEY: %d; shmid %d...\n", pid, SHMKEY, shmid);

    int each = 0;
    while (*pintword == 0) {
        printf("\rWaiting for process to write%.*s   \b\b\b", each, "...");
        fflush(stdout);  // force printing as no newline in output
        sleep(1);
        if (each > 3) {
            each = 0;
        } else {
            each++;
        }
    }

    // print the shared memory segment
    // increment pintword to point to the next element (int*) / word
    //   in the shared memory segment
    killpid = *pintword++;
    printf("got pid: %d to kill()\n", killpid);
    for (i = 1; i < 256; i++) {
        // printf("int*index: %d, value: %d\n", i, *pintword++);
        printf("%d ", *pintword++);
    }
    printf("\n");

    printf("Process [%d] read from shared memory shmid: %d, addr: %p \n", pid, shmid, virtaddr);

    pintword = (int*)virtaddr;
    *pintword = pid;

    printf("Killing pid: %d, leaving pid %d in index 0\n", killpid, pid);

    kill(killpid,1);
}