#include <stdio.h>
#include <unistd.h>
int main() {
    int n = 3; 
    int sum = 0;
    FILE *file;
    
    int j = getpid(); //returns the process id
    printf("getpid = %d\n", (int)getpid());
    fflush(NULL);

    file = fopen("sums", "w");
    if (file == NULL) {
        printf("Error: Unable to open the file!\n");
        return 1;
    }

    for (int i = 1; i <= n; i++) {
        sum += i;
        printf("%d\n", sum);
	fflush(NULL);
        fprintf(file, "%d\n", sum);
        fflush(NULL);
    }

    fclose(file);

    return 0;
}

