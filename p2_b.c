#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

void measure_getpid_time() {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    int pid = getpid();

    gettimeofday(&end, NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds + microseconds*1e-6;

    printf("getpid() time: %f seconds\n", elapsed);
}

void measure_file_operations(int n) {
    struct timeval start, end;

    // Measure getpid() time
    gettimeofday(&start, NULL);
    int pid = getpid();
    gettimeofday(&end, NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double getpid_time = seconds + microseconds*1e-6;
    printf("getpid() time: %f seconds\n", getpid_time);

    // Measure fopen time
    gettimeofday(&start, NULL);
    FILE *file = fopen("sums", "w");
    gettimeofday(&end, NULL);
    seconds = end.tv_sec - start.tv_sec;
    microseconds = end.tv_usec - start.tv_usec;
    double fopen_time = seconds + microseconds*1e-6;
    printf("fopen() time: %f seconds\n", fopen_time);

    if (file == NULL) {
        printf("Error: Unable to open the file!\n");
        return;
    }

    // Measure file write time
    gettimeofday(&start, NULL);
    int sum = 0;
    for (int i = 1; i <= n; i++) {
        sum += i;
        fprintf(file, "%d\n", sum);
    }
    gettimeofday(&end, NULL);
    seconds = end.tv_sec - start.tv_sec;
    microseconds = end.tv_usec - start.tv_usec;
    double fwrite_time = seconds + microseconds*1e-6;
    printf("fprintf() time: %f seconds\n", fwrite_time);

    // Measure fclose time
    gettimeofday(&start, NULL);
    fclose(file);
    gettimeofday(&end, NULL);
    seconds = end.tv_sec - start.tv_sec;
    microseconds = end.tv_usec - start.tv_usec;
    double fclose_time = seconds + microseconds*1e-6;
    printf("fclose() time: %f seconds\n", fclose_time);
}


int main() {
    int values_of_n[] = {1, 1000, 1000000};
    int num_values = sizeof(values_of_n) / sizeof(values_of_n[0]);

    for (int i = 0; i < num_values; i++) {
        int n = values_of_n[i];
        printf("\n\nFor n = %d:\n", n);
        measure_file_operations(n);
    }
    return 0;
}

