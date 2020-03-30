#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<string.h>
#include "extra.h"

#define READ  (thePipe[0])  // index of read end of the pipe
#define WRITE (thePipe[1])  // index of write end of the pipe
#define BUFFER_SIZE 250     // buffer size
void producer(int fd,char* filename);
void consumer(int fd);

//main program: creates pipe
int main(int argc,char *argv[]) {                 //argc for argument count  and argv is argument vector : array of character pointers

  int thePipe[2];       /* stores the descriptor pair */
  pid_t pid1,pid2,pid;  /* process IDs */
  int status;           /* used to collect wait statuses */

  /* checking arguments */
  if (argc<2) {
    fprintf(stderr,"Name of file for the words need!\n");
    fflush(stderr);
    exit(EXIT_FAILURE);
  }
  
  
  
  
  
  
  
  
  
  
  
  //producer process 
 
void producer(int pipeWriteSide,char* filename) {
  
  static char buffer[BUFFER_SIZE];
  static char word[BUFFER_SIZE];
  FILE *instream;
  int counter=0;
  signal(SIGPIPE,SIG_IGN);      //to ignore the SIGPIPE exception when occured 
  
  instream=fopen(filename,"r");
  if (instream==NULL) {
    fprintf(stderr,"Can't open %s\n",filename);
    fflush(stderr);
    close(pipeWriteSide);   // close pipe if error is there
    exit(EXIT_FAILURE);
  }

  
 
