#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdint.h> 
#include <time.h> 
#include <stdbool.h>

#define SIZE 84 // size of array
#define SHM_KEY 1234 // key for the shared memory segment shared among P0, HP1, and HP2


struct shared_struct{
    int arr[9][9]; //this is the return address by shared mmemory attach function and so it can serve as the base address of array
    bool written_by_p , written_by_hp1 , written_by_hp2;
    bool hp1_out, hp2_out;
};
