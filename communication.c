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
    fprintf(stderr,"Unable to open %s\n",filename);
    fflush(stderr);
    close(pipeWriteSide);   // close pipe if error is there
    exit(EXIT_FAILURE);
  }
  
  while(fgets(buffer,BUFFER_SIZE,instream)) {          // for reading words
   
    if (sscanf(buffer,"%[a-zA-Z1-9./]",word)==1) {          // for reading and sending to consumer
      int len_word = strlen(word)+1;                       //+1 for null character
      
	  //  for sending  size of the word 
      if(write(pipeWriteSide,(char*)&len_word,sizeof(int)) != sizeof(int)) 
          break;
      
      if(write(pipeWriteSide,word,len_word) != len_word)
  	  break;
      
      counter++;
    }
  }
   printf("Words sent by producer : %d words\n",counter);               // print counts of words
}
// consumer process start
void consumer(int pipeReadSide) {
  static char buffer[BUFFER_SIZE];// creating buffer
  int bytes,counter,len,left,j;
  
  int cracked[numaccts];

  for(j=0;j<numaccts;j++)
    cracked[i]=0;
  left=numaccts;
  counter=0;
  printf("%d accounts to crack\n",left);
  fflush(stdout);
  
  while(bytes!=0 && left>0);
  printf("%d accounts cracked, %d not cracked, %d words tried\n",
         numaccts-left,left,counter);                                      //prints results
  fflush(stdout);

}

  
 
