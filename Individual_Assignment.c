#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
 
#define PIN_LENGTH 5
#define PIN_WAIT_INTERVAL 2
 
void getPIN(char pin[PIN_LENGTH + 1]) {
  srand(getpid() + getppid());
 
  pin[0] = 34 + rand() % 4;
 
  for(int i = 1; i < PIN_LENGTH; i++) {
    pin[i] = 33 + rand() % 4;
  }
 
  pin[PIN_LENGTH] = '\0';
}

void sigint_handler(int sig)
{
printf("\n Your process has been interupted\n");
exit (1);
}
 
int main(void) {
  while(1) {
    int pipefds[2];
    char pin[PIN_LENGTH + 1];
    char buffer[PIN_LENGTH + 1];
    signal(SIGINT,sigint_handler);
    
    pipe(pipefds);
 
    pid_t pid = fork();
 
    if(pid == 0) {
      getPIN(pin); // generate PIN
      close(pipefds[0]); // close read fd
      write(pipefds[1], pin, PIN_LENGTH + 1); // write PIN to pipe
 
      printf(" Child sending your number to Parent...\n");
 
      sleep(PIN_WAIT_INTERVAL); // delaying PIN generation intentionally.
 
      exit(EXIT_SUCCESS);
    }
 
    if(pid > 0) {
      wait(NULL); // waiting for child to finish
      int y;
      printf("Please enter an integer : ");
      scanf("Your integer: %d", &y);
      close(pipefds[1]); // close write fd
      read(pipefds[0], buffer, PIN_LENGTH + 1); // read PIN from pipe
      close(pipefds[0]); // close read fd
      printf("Parent finally get PIN '%s' from child.\n\n", buffer);
      
        
    }
  }
 
  return EXIT_SUCCESS;
}
