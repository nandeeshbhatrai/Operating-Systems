// From "Advanced Linux Programming"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include<wait.h>

/* Write COUNT copies of MESSAGE to STREAM, pausing for two seconds between each.  */

void writer(const char message, int count, FILE* stream){
  for(; count > 0; --count){
    /* Write the message to the stream, and send it off immediately.  */
    // printf("W: To write\n");
    fprintf(stream, "%C\n", message);
    fflush(stream);
    /* Snooze a while.  */
    // printf("W: To Sleep\n");
    // sleep(2);
  }
}

/* Read random strings from the stream as long as possible.  */
char reader(FILE* stream){
  char buffer[1024];
  /* Read until we hit the end of the stream.  fgets reads until
     either a newline or the end-of-file.  */
  while(!feof(stream) && !ferror(stream) && fgets(buffer, sizeof(buffer), stream) != NULL){ 
    // printf("R: Message is:\n");
    fputs(buffer, stdout);
  }
  return buffer[0];
}

int main(){

	int file_desc1[2];
	int file_desc2[2];
	
	// Create a pipe, file descriptors for the two ends of the pipe are placed in file_desc1
  pipe(file_desc1);
  pipe(file_desc2);
	
	// Create a child
	pid_t pid1 = fork();
	
	// Child process reads from pipe
	if(pid1 == 0){    // child
    char buffer;
    printf("Prisoner1:\nWhat would you do? [1: Testify / other: Remain silent]  ");
    scanf("%c " , &buffer);
    printf("%c " ,buffer);
    // write at pipe
    FILE* stream = fdopen(file_desc1[1], "w");
    writer(buffer , 1 , stream);
    // fprintf(stream, "%c\n", buffer);
    // close(file_desc1[1]);
    // fflush(stream);
    exit(0);
  }else{ // Parent: writes to pipe
    int prisoner1;
    waitpid(pid1 , &prisoner1 , 0);
    printf("DONE PRISONER 1!");

    // read prisoner1
    FILE* stream = fdopen(file_desc1[0], "r");
    char prisoner1_res = reader(stream);
    // while(!feof(stream) && !ferror(stream) && fgets(prisoner1_res, sizeof(prisoner1_res), stream) != NULL){ 
    //   fputs(prisoner1_res, stdout);
    // }
    // close(file_desc1[0]);

    pid_t pid2 = fork();
    if(pid2 == 0){
      char buffer;
      printf("Prisoner2:\nWhat would you do? [1: Testify / other: Remain silent]  ");
      scanf("%c" , &buffer);
      // write at pipe
      FILE* stream = fdopen(file_desc2[1], "w");
      writer(buffer , 1 , stream);
      // fprintf(stream, "%c\n", buffer);
      // close(file_desc1[1]);
      // fflush(stream);
      exit(0);
    }else{
      int prisoner2;
      waitpid(pid1 , &prisoner2 , 0);
      printf("%d and %d\n" , prisoner1 , prisoner2);

      // read prisoner2
      FILE* stream = fdopen(file_desc1[0], "r");
      char prisoner2_res = reader(stream);

      printf("%c , %c" , prisoner1_res , prisoner2_res);
    }
  }

  return 0;
}
