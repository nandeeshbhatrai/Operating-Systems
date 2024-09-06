#include <stdio.h>
#include <time.h>

int main() {
    clock_t time; // initilisation
    time = clock(); // begin time
    printf("Hello, World!\n");
    time = clock() - time; // endtime - begintime = time elapsed
    printf("Time taken to run hello = %f ms\n",((double)time / CLOCKS_PER_SEC)*1e3); // conversion to miliseconds
    return 0;
}

// Here we used the clock() function to calculate the execution time of the code in mili-seconds.
// Alternatively, we can use the "time" utility to calculate the execution time as well.
//
// >> time ./sort
// >> Array size = 100000
//
// >> real	0m4.849s
// >> user	0m4.831s
// >> sys	0m0.012s
//
// Hence, we can find the execution time like this as well.
