/*
Name: Nandeesh Bhatrai
Roll: 2203121

*/



/* Process P0  */
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdint.h> 
#include <time.h> 
#include <stdbool.h>
#include "shm_common.h"

//this function just stores a valid solution
void store_a_valid_solution(int arr[9][9]){
	// Store the values starting from index 1
	arr[0][0] = 5; arr[0][1] = 3; arr[0][2] = 4; arr[0][3] = 6; arr[0][4] = 7; arr[0][5] = 8; arr[0][6] = 9; arr[0][7] = 1; arr[0][8] = 2;
	arr[1][0] = 6; arr[1][1] = 7; arr[1][2] = 2; arr[1][3] = 1; arr[1][4] = 9; arr[1][5] = 5; arr[1][6] = 3; arr[1][7] = 4; arr[1][8] = 8;
	arr[2][0] = 1; arr[2][1] = 9; arr[2][2] = 8; arr[2][3] = 3; arr[2][4] = 4; arr[2][5] = 2; arr[2][6] = 5; arr[2][7] = 6; arr[2][8] = 7;
	arr[3][0] = 8; arr[3][1] = 5; arr[3][2] = 9; arr[3][3] = 7; arr[3][4] = 6; arr[3][5] = 1; arr[3][6] = 4; arr[3][7] = 2; arr[3][8] = 3;
	arr[4][0] = 4; arr[4][1] = 2; arr[4][2] = 6; arr[4][3] = 8; arr[4][4] = 5; arr[4][5] = 3; arr[4][6] = 7; arr[4][7] = 9; arr[4][8] = 1;
	arr[5][0] = 7; arr[5][1] = 1; arr[5][2] = 3; arr[5][3] = 9; arr[5][4] = 2; arr[5][5] = 4; arr[5][6] = 8; arr[5][7] = 5; arr[5][8] = 6;
	arr[6][0] = 9; arr[6][1] = 6; arr[6][2] = 1; arr[6][3] = 5; arr[6][4] = 3; arr[6][5] = 7; arr[6][6] = 2; arr[6][7] = 8; arr[6][8] = 4;
	arr[7][0] = 2; arr[7][1] = 8; arr[7][2] = 7; arr[7][3] = 4; arr[7][4] = 1; arr[7][5] = 9; arr[7][6] = 6; arr[7][7] = 3; arr[7][8] = 5;
	arr[8][0] = 3; arr[8][1] = 4; arr[8][2] = 5; arr[8][3] = 2; arr[8][4] = 8; arr[8][5] = 6; arr[8][6] = 1; arr[8][7] = 7; arr[8][8] = 9;
}

// Function to check if an array "arr1" of size 9 contains all digits from 1 to 9. It is assumed that array elements are from 1 to 9.
bool check_1_to_9(int arr1[]){
    bool digits[9] = {false}; //this array records presence of a digit
    
    for (int i = 0; i < 9; i++) {
        if (digits[arr1[i] - 1]) {
            return false; // a duplicate, all digits will not be present
        }
        digits[arr1[i] - 1] = true; // record the presence of this digit
    }
    return true; // all digits from 1 to 9 are found
}

//this function prints the Sudoku
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
	/*
		YOUR CODE HERE
	*/
    int shmid = shmget((key_t)1234 , sizeof(struct shared_struct) , 0666| IPC_CREAT);
    if(shmid == -1){
        printf("Some Error Occured while connecting to shmid!\n");
        exit(EXIT_FAILURE);
    } 
    void* shared_memory = shmat(shmid , (void*)0 , 0);
    if((unsigned long)(shared_memory) == -1){
        printf("Some Error Occured while shmat!\n");
        exit(EXIT_FAILURE);
    }
    printf("Memory attached at %lX.\n" , (unsigned long) (uintptr_t) shared_memory);
    struct shared_struct* shared_stuff = (struct shared_struct*) shared_memory;

    ///////////////////////////////////////////////////////////////////////////////////////////////////
		
	// Create a random sudoku
    srand(time(0));
    for (int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; ++j){
            shared_stuff -> arr[i][j] = rand() % 9 + 1;
        }
    }

    //Uncomment following function to store a valid solution in array
    store_a_valid_solution(shared_stuff->arr);
        
    //uncomment to print Sudoku
    print_sudoku(shared_stuff->arr);

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    shared_stuff -> written_by_p = true; // P has generated the sudoku!
    
    while(!(shared_stuff -> written_by_hp1) || !(shared_stuff -> written_by_hp2)){
        // wait for it to finish
        if(rand() % 23 == 0){
            printf("\nWaiting for hp1 and hp2 to write!...\n\n");
            sleep(5);
        }
    }

    bool valid = true;

    if(shared_stuff -> hp1_out == 0 || shared_stuff -> hp2_out == 0){
        printf("The sudoku solution is Invalid!\n");
        valid = false;
        shared_stuff->written_by_p = false;
        // detach
        if (shmdt(shared_stuff) == -1) {
            fprintf(stderr, "shmdt failed\n");
            fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
            exit(EXIT_FAILURE);
        }
        
        // Delete
        if (shmctl(shmid, IPC_RMID, 0) == -1) { //Assuming shmid is the shared memeroy identifier
            fprintf(stderr, "shmctl(IPC_RMID) failed\n");
            fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }

    // if both sent "true"
    // check for squares
    for(int i = 0; i < 9; i += 3){
        for(int j = 0; j < 9; j += 3){
            // (i,j) is the top left corner of the square
            bool digits[9] = {false};
            for(int k = 0; k < 3; ++k){
                for(int l = 0; l < 3; ++l){
                    if(digits[shared_stuff->arr[i+k][j+l] - 1]){// already present
                        if(valid){
                            printf("The sudoku solution is Invalid!\n");
                            shared_stuff->written_by_p = false;
                            // detach
                            if (shmdt(shared_stuff) == -1) {
                                fprintf(stderr, "shmdt failed\n");
                                fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
                                exit(EXIT_FAILURE);
                            }
                            
                            // Delete
                            if (shmctl(shmid, IPC_RMID, 0) == -1) { //Assuming shmid is the shared memeroy identifier
                                fprintf(stderr, "shmctl(IPC_RMID) failed\n");
                                fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
                                exit(EXIT_FAILURE);
                            }
                            exit(EXIT_SUCCESS);
                        }
                    }
                    digits[shared_stuff->arr[i+k][j+l] - 1] = true;
                }
            }
        }
    }

    // if all squares are valid:
    printf("**** The sudoku solution is VALID! ****\n");
    
    shared_stuff->written_by_p = false; // reset the flag
	
	// detach
    if (shmdt(shared_stuff) == -1) {
        fprintf(stderr, "shmdt failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }
    
    // Delete
    if (shmctl(shmid, IPC_RMID, 0) == -1) { //Assuming shmid is the shared memeroy identifier
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");
        fprintf(stderr, "Error: %s\n", strerror(errno)); // Print error using strerror
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
