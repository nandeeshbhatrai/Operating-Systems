#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>

void log_command(int pid, int ppid, const char *cmd, int status)
{
    FILE *log_file = fopen("log_my_exec", "a");
    if (log_file == NULL)
    {
        printf("Failed to open log file");
        exit(1);
    }

    struct timeval tv;
    struct tm *ptm;
    char time_string[40];
    long long milliseconds;

    gettimeofday(&tv, NULL);

    // localtime is a C library function which returns struct tm object which is of type time_t when provided with time elapsed since epoch. 
    ptm = localtime(&tv.tv_sec); 


    // strftime converts the struct tm object into human readable date-time format
    strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", ptm);

    milliseconds = tv.tv_usec / 1000;

    fprintf(log_file, "PID: %d, PPID: %d, Command: %s, Exit Status: %d, Time: %s.%03ld\n",
            pid, ppid, cmd, status, time_string, milliseconds);

    fclose(log_file);
}

int main(int args, char *argv[])
{

    int pid = getpid();
    int ppid = getppid();
    int rc = fork();

    if(rc==0){
        // child process
        execvp(argv[1], &argv[1]);
        exit(1);
    }
    else{
        // Parent process
        int status;
        waitpid(rc, &status, 0);
        log_command(pid, ppid, argv[1], WEXITSTATUS(status));
    }

    return 0;
}