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
#define DOCSIZE 10000
#define USERS 100
#define BUFFER_SIZE 256    // buffer size
void producer(int fd,char* filename);
void consumer(int fd);
struct Account {
	char *id;
	char *password;
};
int numaccts;
struct Account  accounts[USERS];
char text[DOCSIZE];
int readAccounts(const char* file_name)
{
	int fd1;
	fd1 = open(file_name,O_RDONLY);
	int n = read(fd1,text,DOCSIZE);
	char* data = strdup(text);
	char* line;
	int i=0;
	while((line = strsep(&data,"\n")) != NULL)
	{
		if(strlen(line) > 0){
			int j=0;
			char* word;
			char temp[256];
			word= strsep(&line," ");
			accounts[i].id = strdup(word);
			word =strsep(&line,"\n");
			strcpy(temp,word);
			temp[strlen(temp) - 1] = '\0';
			word = strdup(temp);
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
		char* Acc_file[256];
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
			producer(thePipe[1],argv[1]);	
        }
    else
    {
		pid2 = fork();
		if(pid2 == 0){
		consumer(thePipe[0]);
    	}	
    	else
    	{
      			wait(NULL);            // now we wait for the 2 children which are working
	  			fflush(stdout);
  				/* normal wait for the second process */
     			pid=wait(NULL);
     			//printf("Process %d has completed!\n",(int)pid);
     			fflush(stdout);
     		return 0;
 		}
 	wait(NULL);
 	}
 return 0;
 
}
  //producer process 
 
void producer(int pipeWriteSide,char* filename) {
   static char buffer[BUFFER_SIZE];
  
   int *instream;
   int counter=0;
  instream = open(filename,O_RDONLY);
  if (instream < 0) {
    	fprintf(stderr,"Unable to open %s\n",filename);
    	fflush(stderr);
    	close(pipeWriteSide);   // close pipe if error is there
    	exit(EXIT_FAILURE);
  	}
  	char* data;
  	int n = read(instream,data,10000);
  	char* word;
   while((word = strsep(&data,"\n")) != NULL) {          // for reading words
   
    	if (strlen(word) > 0) {  // for reading and sending to consumer,  
      		int len = strlen(word);                       //storing length
      
	  				//  for sending  size of the word 
      				if(write(pipeWriteSide,(char*)&len,sizeof(int)) != sizeof(int)) 
          					break;
      	if(write(pipeWriteSide,w,len) != len)
  	 			 break;
      	counter++;    // count the words
    }
    else
    {
    	break;
	}
  }
   int len = 0;
   write(pipeWriteSide,(char*)&len,sizeof(int)) != sizeof(int);
   printf("Words sent by producer : %d words\n",counter);               // print counts of words
}
// consumer process start:::
int guess(int i, char *passwd)
{
	if(strcmp(accounts[i].password,passwd) == 0)
	{
		return 1;
	}
	return 0;
}
void consumer(int pipeReadSide) {
  		static char buffer[BUFFER_SIZE];// creating buffer
  		int bytes,counter,len,remaining,i;
  		int cracked[numaccts];
  		for(i=0;i<numaccts;i++)
    			cracked[i]=0;
    	remaining = numaccts;
    	counter=0;
    	printf("%d accounts to crack\n",remaining);
    	fflush(stdout);
  		do {
    			// word length  
    			bytes =read(pipeReadSide,(char*)&len,sizeof(int));

    			if (bytes>0) {
						// then the word 
      					bytes=read(pipeReadSide,buffer,len);
      					buffer[bytes] = '\0'
						// if something read 
     					 if (bytes>0) {
       							 counter++;
    							if (counter%500==0) {
          									printf("%d  number of words tried till now......\n",counter);
          									fflush(stdout);
       							}
        						for(i=0;i<numaccts;i++)
         								 //if not cracked till then crack now 
          								if (!cracked[i] && guess(accounts[i],buffer)) {
            										cracked[i]=1;
            										remaining--;
            									printf("Account %s cracked, password=%s\n",i,buffer);
                   
            									fflush(stdout);
          								}
        
      						} // close of if (b>0)
      
        
      			}      //close of first if (bytes>0)
      			else
      					break;
    } 

   while(bytes!=0 && remaining>0);
   close(PipeReadSide);
   printf(" accounts cracked:%d, not cracked:%d, words tried:%d \n",
         numaccts-remaining,remaining,counter);                                      //prints results
   fflush(stdout);

}

  
 
