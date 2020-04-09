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
#define DOCSIZE 1000
#DEFINE USERS 100
#define BUFFER_SIZE 256    // buffer size
void producer(int fd,char* filename);
void consumer(int fd);
struct Account{
	char *id;
	char *password;
};
int numaccts;
struct Account accounts[USERS];
char text[DOCSIZE];
int readAccounts(const char* file_name)
{
	int fd1;
	fd1=open(file_name,O_RDONLY);
	int n= read(fd1,text,DOCSIZE);
	char* data =strdup(text);
	char* line;
	int i=0;
	while((line = strsep(&data,"\n")) != NULL)
	{
		if(strlen(line)>0){
			int j=0;
			char* word;
			word= strsep(&line," ");
			accounts[i].id = strdup(word);
			word =strsep(&line,"\n");
			accounts[i].password = strdup(word);
			i++;
		}
		else 
		    break;
	}
	return i;
}
//main program: creates pipe
int main(int argc,char *argv[]) {                 //argc for argument count  and argv is argument vector : array of character pointers
		char* Acc_file;
		printf("ENTER aCCOUNTS FILE NAME\n");
		scanf("%s",Acc_file);
		numaccts = readAccounts(Acc_file);

  int thePipe[2];       /* stores the descriptor pair */
  pid_t pid1,pid2,pid;  /* process IDs */
  int status;           /* used to collect wait statuses */

  /* checking arguments */
  if (argc<2) {
    fprintf(stderr,"Name of file for the words need!\n");
    fflush(stderr);
    exit(EXIT_FAILURE);
  }
pipe(thePipe);
pid1 =fork();
if(pid1 == 0){	
producer(thePipe[1],"words3.txt");	
}
pid2 = fork();
if(pid2 == 0){
	consumer(thePipe[0]);
}
wait(NULL);            // now we wait for the 2 children which are working
wait(NULL);
  
  
  //we should be careful that every process is dead when program exits
  //waitpid() waits only for terminated children, to change this behaviour via the options argument such as WIFEXITED, WEXITSTATUS is used.
  pid=wait(&status);
  if (WIFEXITED(status)) {					
      printf("status: exit(%d)\n",
             WEXITSTATUS(status));
      fflush(stdout);
  }

  // if abnormal termination is there
  if (!(WIFEXITED(status) && WEXITSTATUS(status)==0)) {
    // to kill the other process 
    if (pid==pid1)
      kill(pid2,2);
    else if (pid==pid2)
      kill(pid1,2);
    
	// waiting for the second one 
    wait(NULL);
    fprintf(stderr,"Error is there so program stopped!\n");
    fflush(stderr);
    exit(EXIT_FAILURE);
  }
 printf("Process %d has finished!\n",(int)pid);
  fflush(stdout);
  /* normal wait for the second process */
  pid=wait(NULL);
  printf("Process %d has completed!\n",(int)pid);
  fflush(stdout);
  return 0;

}
  //producer process 
 
void producer(int pipeWriteSide,char* filename) {
   static char buffer[BUFFER_SIZE];
  static char w[BUFFER_SIZE];
  FILE *instream;
  int counter=0;
  signal(SIGPIPE,SIG_IGN);      //to ignore the SIGPIPE exception when occured 
  
  instream=open(filename,O_RDONLY);
  if (instream < 0) {
    fprintf(stderr,"Unable to open %s\n",filename);
    fflush(stderr);
    close(pipeWriteSide);   // close pipe if error is there
    exit(EXIT_FAILURE);
  }
  char* data;
  int n =read(instream,data,10000);
  char* word;
  
  while(fgets(buffer,BUFFER_Size,instream)) {          // for reading words
   
    if (strlen(word) > 0) {  // for reading and sending to consumer,  
											

      int len = strlen(word);                       //storing length
      
	  //  for sending  size of the word 
      if(write(pipeWriteSide,(char*)&len,sizeof(int)) != sizeof(int)) 
          break;
      if(write(pipeWriteSide,w,len) != len)
  	  break;
      counter++;
    }
    else
    	break;
  }
   printf("Words sent by producer : %d words\n",counter);               // print counts of words
}
// consumer process start:::
int guess(int i, char *passwd)
{
	if(strcmp(accounts[i].password,passwd) == 0)
		return 1;
	return 0;
}
void consumer(int pipeReadSide) {
  static char buffer[BUFFER_SIZE];// creating buffer
  int b,counter,len,left,j;
  int cracked[numaccts];
  for(j=0;j<numaccts;j++)
    	cracked[j]=0;
  left=numaccts;
  counter=0;
  printf("%d accounts to crack\n",left);
  fflush(stdout);
  do {
    // word length  
    b=read(pipeReadSide,(char*)&len,sizeof(int));

    if (b>0) {

      if (b!=sizeof(int)) {
			if (b<0)
	 			 perror("read");
			else {
	 				 fprintf(stderr,"read(int) failed (%d read)\n",b);
	  				 fflush(stderr);
			}
	//closing the pipe
        close(pipeReadSide);
	//error
        exit(EXIT_FAILURE);
      	}

      // then the word 
      b=read(pipeReadSide,buffer,len);

      // if something read 
      if (b>0) {
        counter++;
        printf("Try %s\n",buffer); 
        if (counter%500==0) {
          printf("%d  number of words tried till now......\n",counter);
          fflush(stdout);
        }
        for(i=0;i<numaccts;i++)
          //if not cracked till then crack now 
          if (!cracked[i] && guess(accounts[i],buffer)) {
            cracked[i]=1;
            left--;
            printf("Account %s cracked, password=%s\n",
                   accounts[i],buffer);
            fflush(stdout);
          }
        
      } // close of if (b>0)
      // else if error is there then this part run
      else if (b<0) {
        perror("read");
        //closing the pipe 
        close(pipeReadSide);
        // error code 
        exit(EXIT_FAILURE);
      }
    } //close of first if (bytes>0)

}
   while(bytes!=0 && left>0);
  printf(" accounts cracked:%d, not cracked:%d, words tried:%d \n",
         numaccts-left,left,counter);                                      //prints results
  fflush(stdout);

}

  
 
