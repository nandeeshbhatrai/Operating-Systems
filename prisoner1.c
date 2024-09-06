/* Producer  */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdint.h> // For uintptr_t
#include <time.h> // Include this header for time()
#include "shm_common.h"

int main(){
	srand(time(NULL));
	printf("PID: %d\n", (int)getpid());
	
	//get shared mem identifier of a new or existing shared memory
    int shmid = shmget((key_t)1234, sizeof(struct shared_struct), 0666 | IPC_CREAT);

    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }

	// attach
    void *shared_memory = shmat(shmid, (void *)0, 0);
    
    if (shared_memory == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }
    printf("Memory attached at %lX\n",  (unsigned long)(uintptr_t)shared_memory);
    
    // in infinite loop: write something or wait for consumer to read
    
    int running = 1;

    struct shared_struct *shared_stuff = (struct shared_struct *)shared_memory;
	    
    shared_stuff->written_by_prod = 0; //reset the flag
    
    char buffer[BUFSIZ]; //data of BUFSIZ, obtained from stdin to be written to shared mem
    
    while(running) {
        while(shared_stuff->written_by_prod == 1) {
            sleep(1);
        	if(rand()%20 == 0){
        		printf("\nWaiting for consumer to read\n\n");
        	}
        }
        printf("Prisoner1:\nDo you testify your crime or remain silent?[1: Testify / other: Remain silent] ");
        fgets(buffer, BUFSIZ, stdin);

        if(strncmp(buffer , "1" , 1) == 0){
            printf("You've testifid for the crimes.\n");
        }else{
            printf("You've decided to remain silent.\n");
        }
        
        strncpy(shared_stuff->data, buffer, SIZE);
        shared_stuff->written_by_prod = 1;

        running = 0;
    }
	
	// detach
    if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "shmdt failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
