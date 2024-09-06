/* Consumer */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdint.h> // For uintptr_t
#include <time.h> // Include this header for time()
#include "shm_common.h"

char prisoner1_res[2048], prisoner2_res[2048];

int main(){
	srand(time(NULL));
	printf("PID: %d\n", (int)getpid());
	
	//Step 1. Allocate - shmget
	
	int shmid1 , shmid2;	//shared memory id, returned from shmget
	
	shmid1 = shmget((key_t)1234, sizeof(struct shared_struct), 0666 | IPC_CREAT);
	shmid2 = shmget((key_t)12345, sizeof(struct shared_struct), 0666 | IPC_CREAT);

    if (shmid1 == -1) {
        fprintf(stderr, "shmget1 failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }else if (shmid2 == -1) {
        fprintf(stderr, "shmget2 failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }
    
    
    //Step 2. Attach - shmat1
	
	void *shared_memory1 = shmat(shmid1, (void *)0, 0);	//address of first byte of the shared memory, returned from shmat
    
    if (shared_memory1 == (void *)-1) {
        fprintf(stderr, "shmat1 failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }

    printf("Memory attached at %lX\n",  (unsigned long)(uintptr_t)shared_memory1);

    //Step 2. Attach - shmat2

    void *shared_memory2 = shmat(shmid2, (void *)0, 0);	//address of first byte of the shared memory, returned from shmat
    
    if (shared_memory2 == (void *)-1) {
        fprintf(stderr, "shmat2 failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }

    printf("Memory attached at %lX\n",  (unsigned long)(uintptr_t)shared_memory2);

	
	// In the following, the consumer will read the shared memory, the data written by producer, - in an infinite loop until producer sends "end"
	
	// int running = 1;	//program runs in an infinite loop till running is 1
	
	// shared_stuff points to shared_memory1 segment
	
	struct shared_struct *shared_stuff1  = (struct shared_struct *)shared_memory1; //shared structure1
	struct shared_struct *shared_stuff2  = (struct shared_struct *)shared_memory2; //shared structure2
	
	// reset flag, do this in producer also
	shared_stuff1->written_by_prod = 0;
	shared_stuff2->written_by_prod = 0;

    int received_responses = 0;
	
	// infinite loop- sleep or read data if available
	while(received_responses < 2) {
        if (shared_stuff1->written_by_prod){
            strcpy(prisoner1_res , shared_stuff1->data);
            printf("Received from prisoner1: %s\n", (strncmp(shared_stuff1->data, "1" , 1) ? "Remain Silent!" : "Testify!"));
            shared_stuff1->written_by_prod = 0;
            ++received_responses;
        }
        if (shared_stuff2->written_by_prod){
            strcpy(prisoner2_res , shared_stuff2->data);
            printf("Received from prisoner2: %s\n", (strncmp(shared_stuff2->data, "1" , 1) ? "Remain Silent!" : "Testify!"));
            shared_stuff2->written_by_prod = 0;
            ++received_responses;
        }
        if(received_responses < 2){
            if(rand()%20 == 0){
                printf("\nWaiting for producer\n\n");
            }
        	sleep(1);
        }
    }

	// Step 3. Detach
	
    if (shmdt(shared_memory1) == -1) {
        fprintf(stderr, "shmdt1 failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }
    if (shmdt(shared_memory2) == -1) {
        fprintf(stderr, "shmdt2 failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }

	// Step 4. Delete
	
    if (shmctl(shmid1, IPC_RMID, 0) == -1) {
        fprintf(stderr, "1: shmctl(IPC_RMID) failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }
    if (shmctl(shmid2, IPC_RMID, 0) == -1) {
        fprintf(stderr, "1: shmctl(IPC_RMID) failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }

    // Warden's decision
    printf("\nFinal judgement:\n");
    if(strncmp(prisoner1_res , "1" , 1) == 0 && strncmp(prisoner2_res , "1" , 1) == 0){
        printf("Prisoner1 gets: 2 year imprisonment!\nPrisoner2 gets: 2 year imprisonment!\n");
    }else if(strncmp(prisoner1_res , "1" , 1) == 0){
        printf("Prisoner1 gets: FREE!\nPrisoner2 gets: 3 year imprisonment!\n");
    }else if(strncmp(prisoner2_res , "1" , 1) == 0){
        printf("Prisoner1 gets: 3 year imprisonment!\nPrisoner2 gets: FREE!\n");
    }else{
        printf("Prisoner1 gets: 1 year imprisonment!\nPrisoner2 gets: 1 year imprisonment!\n");
    }

    exit(EXIT_SUCCESS);
}

