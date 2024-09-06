/*
Name: Nandeesh Bhatrai
Roll: 2203121

*/



/* Helper Process P1  */

#include "shm_common.h"

void print_sudoku(int arr[9][9]){
	printf("\t\tPrinting Sudoku\n");
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			printf("%d\t", arr[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

int main(){
    int shmid = shmget((key_t)1234 , sizeof(struct shared_struct) , 0666| IPC_CREAT);
    if(shmid == -1){
        printf("Some Error Occured while connecting to shmid!");
        exit(EXIT_FAILURE);
    } 
    void* shared_memory = shmat(shmid , (void*)0 , 0);
    if((unsigned long)(shared_memory) == -1){
        printf("Some Error Occured while shmat!");
        exit(EXIT_FAILURE);
    }
    printf("Memory reading from %lX.\n" , (unsigned long) (uintptr_t) shared_memory);
    struct shared_struct* shared_stuff = (struct shared_struct*) shared_memory;

    // wait for the parent to generate the sudoku:
    while(!(shared_stuff->written_by_p)){
        // wait for it to generate first
        if(rand() % 23 == 0){
            printf("\nWaiting for P to generate sudoku!...\n");
            sleep(3);
        }
    }

    // if it has been generated
    // check for rows    
    for(int i = 0; i < 9; ++i){
        bool digits[10] = {false};
        //check for ith row
        for(int j = 0; j < 9; ++j){
            if(digits[shared_stuff-> arr[i][j]]){
                shared_stuff->hp1_out = false;
                shared_stuff->written_by_hp1 = true;
                printf("error by hp1\n");
                exit(EXIT_SUCCESS);
            }
            digits[shared_stuff->arr[i][j]] = true;
        }
    }

    // if all rows are correct:
    shared_stuff -> hp1_out = true;
    shared_stuff->written_by_hp1=true;

    printf("hp1 Success!\n");
    
	
	// detach
    if (shmdt(shared_stuff) == -1) {
        fprintf(stderr, "shmdt failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
