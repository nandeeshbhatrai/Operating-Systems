// helper code to generate test case

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int n, a, b, c, d;
    FILE *file;
    
    // Seed the random number generator
    srand(time(NULL));
    
    // Read input values
    printf("Enter the value of n: ");
    scanf("%d", &n);

    // Open the file for writing
    file = fopen("process_file", "w");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }
    
    // Write n and t to the file
    fprintf(file, "%d\n", n);

    // Generate and write values to the file
    int prev = 0;
    for (int i = 0; i < n; i++) {        
        fprintf(file, "%d\n", prev); // arrival time
        prev += rand()%6;
        fprintf(file, "%d\n", 1 + rand()%10); // burst time
    }

    // Close the file
    fclose(file);
    
    return 0;
}

