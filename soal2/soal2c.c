#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 

int main() 
{ 


	int pipe0[2]; // Used to store two ends of first pipe 
	int pipe1[2]; // Used to store two ends of second pipe 
	
	char *arg2[] = {"sort", "-nrk", "3,3", NULL};
	char *arg1[] = {"ps", "aux", NULL};
	char *arg3[] = {"head", "-5", NULL};
	
	pid_t p_id1, p_id2; 
	
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
	
	
	//ps aux | sort -nrk 3,3 | head -5	
	
	p_id1 = fork(); 
	if (p_id1 < 0) 
	{ 
		fprintf(stderr, "fork Failed" ); 
		return 1; 
	}  
	
	if (p_id1 == 0) 
	{ 
        	p_id2 = fork();
        	
        	if (p_id2 == 0) {
        	close(pipe0[0]);
        	close(pipe1[1]);
            	close(pipe1[0]);
        	
        	dup2(pipe0[1], STDOUT_FILENO);
    
	        close(pipe0[1]);
	        	
	        
        	execv("/usr/bin/ps", arg1);
		}  
		else {
			
			close(pipe0[1]);
		        close(pipe1[0]);
		        
		        dup2(pipe0[0], STDIN_FILENO);
		        dup2(pipe1[1], STDOUT_FILENO);
		        
		        close(pipe1[1]);
		        close(pipe0[0]);

		        execv("/usr/bin/sort", arg2);	
		    }
	}
		
	else {
		
        	close(pipe0[1]);
        	close(pipe0[0]);
        			
        	dup2(pipe1[0], STDIN_FILENO);
      		//dup2(pipe1[1], STDOUT_FILENO);
        			
        	close(pipe1[0]);
	        close(pipe1[1]);
		
        	execv("/usr/bin/head", arg3); 
       }
}

