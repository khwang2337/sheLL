#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#define C_SIZE 100

void parse(char * line) {
    int i = 0;
    char* command[C_SIZE];
    
    while (command[i] = strsep(&line, " ")) i++;

    if (! strcmp(command[0],"cd") ) {
      if (chdir(command[1]) == -1) printf("Error: %s", strerror(errno));
    }
    else if (! strcmp(command[0],"exit") ) exit(0);
    else {
      int pid = fork();
      
      if (pid) {
        wait(0);
      }
      else {
        if ( execvp(command[0], command) == -1) {
          if (errno != 2) printf("Error: %s", strerror(errno));
          else printf("Error: Not a command");
        }
        exit(0);
      }
    }
}
  
