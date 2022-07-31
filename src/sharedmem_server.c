#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define SHMKEY 1337
#define KBYTES 1024
#define NUMBYTES 256
#define SHMACCESS 0777

int shmid;

int main() {
    int i;
    int* pintword;
    char* addr;
    extern char* shmat();

    // create a new shared memory segment (shmem table)
    // shmget(1337, 256 * 1024, 0777);
    shmid = shmget(SHMKEY, NUMBYTES * KBYTES, SHMACCESS);
    // attach the shared memory segment to the process's address space
    // (allocates memory, and returns a pointer to the shared memory segment)
    addr = shmat(shmid, 0, 0);

    pintword = (int *) addr;

    while( *pintword == 0 ) {
        printf("Waiting for process to write...\n");
        sleep(1);
    }

    for(i = 0; i < 256; i++) {
        printf("%c", addr[i]);
    }


}