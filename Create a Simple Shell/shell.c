#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128

char prompt[] = "> ";
char delimiters[] = " \t\r\n";
extern char **environ;

char *getcwd(char *buf, size_t size);

char cwd[100];
void sighandler(int);
pid_t pid;

int main() {
		signal(SIGINT, sighandler);
    // stores the string typed into the command line.
    char command_line[MAX_COMMAND_LINE_LEN];
    char cmd_bak[MAX_COMMAND_LINE_LEN];
  
    char *arguments[MAX_COMMAND_LINE_ARGS];
    char *newenv[MAX_COMMAND_LINE_ARGS]; // tokenized command line
  
    int i, j, k, n;
		bool isBackground = false; // flag
    char *test;
    	
    while (true) {
				isBackground = false;      
        do{ 
						if (getcwd(cwd, sizeof(cwd)) != NULL) {
								//printf("%s>", cwd);
						} else {
								perror("getcwd() error");
								return 1;
						}

            printf("%s> ", cwd); // print out prompt
            fflush(stdout);

            // reads input from stdin and store it in command_line. If there's an error, exit immediately.      
            if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
                fprintf(stderr, "fgets error");
                exit(0); }
 
        }while(command_line[0] == 0x0A);  // while just ENTER pressed

      
        // If the user input was EOF (ctrl+d), exit the shell.
        if (feof(stdin)) {
            printf("\n");
            fflush(stdout);
            fflush(stderr);
            return 0;
        }

        // TODO:
        
			  // 0. Modify the prompt to print the current working directory
			  // line 32
			
        // 1. Tokenize the command line input (split it on whitespace)
         
         arguments[0] = strtok(command_line, delimiters);
         // walk through other tokens
      
         i = 0;
         while( arguments[i] != NULL ) {
           arguments[++i] = strtok(NULL, delimiters);
         }
         arguments[i] == NULL;
        
      
        // 2. Implement Built-In Commands
        // const char *pwd = "pwd";
				// printf("cmnd is = %s", cmnd);
        /*int pwd = strcmp(cmnd, "pwd") - 10;
        int echo = strcmp(cmnd, "echo");
        int exit = strcmp(cmnd, "exit") - 10;
        int env = strcmp(cmnd, "env") - 10;
        int set_env = strcmp(cmnd, "setenv");
        */
        if(strcmp(arguments[0], "cd")==0){
					// $
              // printf("%s\n",arguments[0]);
              // test = arguments[0];
              // first element go test is test[0]
              chdir(arguments[1]);
            }
        else if(strcmp(arguments[0], "pwd")==0){
						if (getcwd(cwd, sizeof(cwd)) == NULL) {
									perror("getcwd() error");
									return 1;
							}

							printf("%s\n", cwd); // print out prompt
							fflush(stdout);
						}
        else if(strcmp(arguments[0], "echo")==0){
					// $
					k = 1;
					while(arguments[k]!=NULL){
            printf("%s ", arguments[k]);
						k++;
					}
					printf("\n");
          }
        else if(strcmp(arguments[0], "exit")==0){
            exit(0);
          }
        else if(strcmp(arguments[0], "env")==0){
					// $ maybe
						
            printf("%s\n",getenv(arguments[1]));
          }
        else if(strcmp(arguments[0], "setenv")==0){
					// $
            // printf("setenv function\n");
            n = 0;
            newenv[n]=strtok(arguments[1],"="); 
            while(newenv[n]!=NULL)                    
            {
              printf("newenv [%d]=%s\n",n,newenv[n]);
              n++;
              newenv[n]=strtok(NULL,"=");
            }
            setenv(newenv[0], newenv[1], 1);
          }
       else {
				 if (strcmp(arguments[i-1], "&") == 0) {
					 arguments[i-1] = NULL;
					 isBackground = true; //flag
				 }
        
          // 3. Create a child process which will execute the command line input
          pid = fork();
          if (pid < 0) {
            perror("fork failed"); 
            exit(0);
          }
          else if (pid==0) { // child

            execvp(arguments[0], arguments);
            
            
            exit(0);
          } 
          else{ // parent
            // 4. The parent process should wait for the child to complete unless its a background process
            if (isBackground != true) {
              wait(NULL);
            }
          }
          
        }
  
      
        // Hints (put these into Google):
        // man execvp
        // man environ
        // man signals
        
        // Extra Credit
        // man dup2
        // man open
        // man pipes
    }
    // This should never be reached.
    return -1;
}


void sighandler(int signum) {
   printf("Caught signal %d, coming out...\n", signum);
	 kill(pid, SIGKILL);
}