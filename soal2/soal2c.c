#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 

int main() 
{ 


	int pipe0[2]; // Used to store two ends of first pipe 
	int pipe1[3]; // Used to store two ends of second pipe 
	int pipe2[2]
	pid_t p; 
        int status4;
	if (pipe(pipe0)==-1) 
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 
	if (pipe(pipe1)==-1) 
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 
 	if (pipe(pipe2)==-1) 
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 
	p = fork(); 
	if (p < 0) 
	{ 
		fprintf(stderr, "fork Failed" ); 
		return 1; 
	}  
	else if (p == 0) 
	{ 
        dup2(pipe0[1], 1);

        close(pipe0[0]);
        close(pipe0[1]);

//ps aux | sort -nrk 3,3 | head -5	
	
        char *arg1[] = {"ps", "aux", NULL};
        execv("/bin/ps", arg1);
	
	} 
        else{

        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe1[2]);
		
        dup2(pipe0[0], 0);
        dup2(pipe1[1], 1);

        close(pipe0[0]);
        close(pipe0[1]);

        char *arg2[] = {"sort", "-nrk", "3,3", NULL};
        execv("/usr/bin/sort", arg2);
        
        }
        else{
        
        close(pipe2[0]);
        close(pipe2[1]);
		
        dup2(pipe1[0], 0);
        dup2(pipe2[1], 1);

        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe1[2]);

        char *arg2[] = {"head", "-5", NULL};
        execv("/usr/bin/head", arg2);

} 
