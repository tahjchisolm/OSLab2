#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h> // signals

#define MAX 5

void SIGINT_handler(int sig) { // signal handling functions can't return anything
  
  printf("In Child %d - Caught SIGINT!", getpid());

  exit(1);
}

void main(int argc,  char *argv[]) {
  
  int pid[MAX];
  int status;
  int i;
  
  sigset_t cmask, origmask;
  
  for(i = 0; i < MAX; i++) {
    
    pid[i] = fork();  // creates a child
    
    srandom(getpid());
    
    switch(pid[i]) {
      case 0:     // child
        
        //sigaddset (&cmask, SIGTERM); // if you don't list a function, it will do default
        //sigaddset (&cmask, SIGINT);
        //sigaddset (&cmask, SIGHUP);
        //sigaddset (&cmask, SIGUSR1);
        
        //if (sigprocmask(SIG_BLOCK, &mask, &orig_mask) < 0) { // block signals set up in mask, save current state in original mask
          //printf("error\n");
        //}
        
        // register function to handle signal
        signal(SIGINT, SIGINT_handler); // must come before ls
        printf("In Child, pid = %d, parent pid = %d ... \n", getpid(), getppid());
        wait();
        // sleep(random() % 3); // child sleeps random amount of time
        printf("Child executing \"ls -l\"...\n");
        execl("/bin/ls", "ls", "-l", (char *)0); // children will fork off other children unless....
        break;
      
      case 1:   // error
        break;
        
      default: // parent, PID is child's PID
        break;
    }
  }
  
  for(i=0; i < MAX; i++) {
    // u can send a signal sent to the command line using kill()
    kill(pid[i], SIGINT); // parent will send the signal to all children to print message
    printf("Parent waiting on Child pid = %d...\n", pid[i]);
    wait(&status);  // parent wait until child terminates, then reaps child
    printf("Child pid = %d terminated with status = %d\n", pid[i], status);
  }
  
  printf("Parent terminating! Child status = %d\n", status);
}

