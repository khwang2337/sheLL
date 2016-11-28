#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

// void exec(

void parse(char* line){
    int i = 0;
    char* command[100];
    while (line){
        command [i] = strsep(&line, " ");
        i++;
    }
    command[i] = NULL;

    if (!strcmp(command[0],"cd")){
      chdir(command[1]);
    }
    else if (!strcmp(command[0],"exit")){
      exit(1);
    }
    else{
      int pid = fork();
      if (pid){
        wait(0);
      }
      else{
        int r = execvp(command[0], command);
        if(r == -1){
          printf("error\n");
          exit(0);

        }
        printf("%d\n", r);
      }
    }
}
