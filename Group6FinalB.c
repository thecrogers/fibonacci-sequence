//Programmer: Caleb Rogers, Philip Becker
//Course:  CSCE 36000
//Description:  Establishes process ring with piped communication and fib sequence
//File:   TokenRingExample.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <gmp.h>
#define STD_IN 0
#define STD_OUT 1
#define BUFFERSIZE 20

//Long long unsigned doesn't go above the 94th. Just a heads up. We'd need to use or implement a bigNumber library
//Rather just work on part C for now.
struct fibseq{
  unsigned long long    x;
  unsigned long long    y;
  unsigned long long    z;
  int count ;
   };


	struct fibseq fib;
int main(int argc, char* argv[])
{



	int numberOfProcess,i,tmp,count,Temp;
	pid_t childpid;
	pid_t mypid;
	pid_t currentid;
	int FD[2];   //Reusable Pipe
	struct fibseq *pfib;
	pfib = &fib;
	mypid = getpid();
    fib.count=1;
	//fprintf(stderr," alpha pid: %d\n",mypid);
	if(argc != 2)
	{
		//Check for proper number of command line arguments
		fprintf(stderr, "Usage: %s number of processes\n", argv[0]);
		return 1;
	}

	//Init struct.
	numberOfProcess = atoi(argv[1]);
	fib.x = 0;
 	fib.y = 1;
        fib.z=0;
        Temp=0;
        currentid=getpid();
        if(fib.x==0&&fib.y==0)
         {
          fprintf(stderr, "PID: %d PNum:%d %llu + %llu = %llu \n",currentid, pfib->count,pfib->x,pfib->y, pfib->z);
          fib.x=1;
          Temp=1;
          fib.count++;

         }
	//Establish Original Pipe
	pipe(FD);
	if( dup2(FD[0], STD_OUT) <0)
	{
		fprintf(stderr, "Pipe Error - Error code: %s\n", strerror( errno ) );

	}
	//set stdout to pipe write alpha
	if( dup2(FD[1], STD_OUT) < 0 )
	{
		fprintf(stderr, "Pipe Error - Error code: %s\n", strerror( errno ) );
	}

	write(STD_OUT,pfib,sizeof(*pfib));


	for(i = Temp; i < numberOfProcess; i++)
	{
		childpid = fork();
		if(childpid==-1)
		{
			fprintf(stderr, "Fork Error - Error code: %s\n", strerror( errno ) );
		}

		if(childpid!=0){break;}

	}

	currentid=getpid();
	if(currentid!=mypid)//does not allow alpha to have overriden std_in
	{
		if( dup2(FD[0], STD_IN) < 0)
		{
			fprintf(stderr, "Pipe Error - Error code: %s\n", strerror( errno ) );
		}

		read(STD_IN, pfib, sizeof(*pfib));
		//Do the math here for the fib does not print correct first number.
                fib.z = fib.x + fib.y;
		fprintf(stderr, "PID: %d PNum:%d %llu + %llu = %llu \n",currentid, pfib->count,pfib->x,pfib->y, pfib->z);
                fib.count++;
                fib.x=fib.y;
                fib.y=fib.z;
		write(STD_OUT,pfib, sizeof(*pfib));

	}


	if( currentid==mypid)//parent code.
	{
		sleep(1);

		if(dup2(FD[0], STD_IN) < 0) //connects the alpha parent to last child
		{
			fprintf(stderr, "Pipe Error - Error code: %s\n", strerror( errno ) );
		}
		read(STD_IN, pfib,sizeof(*pfib));
	//	fprintf(stderr, "This is alphaParent process %d BufferIN: %s & BufferOut: %s\n",getpid(),bufferIN, bufferOUT);


		wait(NULL);
		fprintf(stderr, "\n***Child finished***\n");
	}
	//Close created pipe
	close(FD[0]);
	close(FD[1]);
	return 0;
}
