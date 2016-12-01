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

void exec(char * a, char * b, char del) {
    int pid, i = 0;
    char* command[C_SIZE];

    while (command[i] = strsep(&a, " ")) i++;
    //printf("%s\n", command[0]);
    if (! strcmp(command[0],"cd") ) {
      if (chdir(command[1]) == -1) printf("Error: %s", strerror(errno));
    }
    else if (! strcmp(command[0],"exit") ) exit(0);
    else {
      if ( (pid = fork()) == -1) printf("Error: %s", strerror(errno));
      else {
        if (pid) {
          wait(0);
        }
        else {
          if (! del) {
            if ( execvp(command[0], command) == -1) {
              if (errno != 2) printf("Error: %s", strerror(errno));
              else printf("Error: Not a command");
            }
          }
          else {
            if (del == '>'){
              int fd = open(b, O_CREAT|O_RDWR|O_TRUNC, 0644);
              int stdout = dup(1);
              dup2(fd,1);
              if ( execvp(command[0], command) == -1) {
                if (errno != 2) printf("Error: %s", strerror(errno));
                else printf("Error: Not a command");
              }
              dup2(stdout,1);
              return;
            }
            
            if (del == '<'){
              int fd;
              if ( (fd = open(b, O_RDONLY, 0644)) == -1 ) {
                printf("Error: no such file");
                exit(0);
              }
              
              int stdin = dup(0);
              dup2(fd,0);
              
              if ( execvp(command[0], command) == -1) {
                if (errno != 2) printf("Error: %s", strerror(errno));
                else printf("Error: Not a command");
              }
              
              dup2(stdin,0);
              exit(0);
            }
            printf("got here!\n");
          }
        exit(0);
        }
      }
    }
}

void redirect(char * line, char *b){
  int pid, i = 0;
  char* command[C_SIZE];

  while (command[i] = strsep(&line, " ")) i++;
  //printf("%s\n", command[0]);
  if (! strcmp(command[0],"cd") ) {
    if (chdir(command[1]) == -1) printf("Error: %s", strerror(errno));
  }
  else if (! strcmp(command[0],"exit") ) exit(0);
  else {
    if ( (pid = fork()) == -1) printf("Error: %s", strerror(errno));
    else {
      if (pid) {
        wait(0);
      }
      else {
        int fd = open(b, O_CREAT|O_RDWR|O_TRUNC, 0644);
        int stdout = dup(1);
        dup2(fd,1);
        if ( execvp(command[0], command) == -1) {
          if (errno != 2) printf("Error: %s", strerror(errno));
          else printf("Error: Not a command");
        }
        dup2(stdout,1);
        exit(0);
      }
    }
  }
}

void aredirect(char * line, char *b){
  int pid, i = 0;
  char* command[C_SIZE];

  while (command[i] = strsep(&line, " ")) i++;
  //printf("%s\n", command[0]);
  if (! strcmp(command[0],"cd") ) {
    if (chdir(command[1]) == -1) printf("Error: %s", strerror(errno));
  }
  else if (! strcmp(command[0],"exit") ) exit(0);
  else {
    if ( (pid = fork()) == -1) printf("Error: %s", strerror(errno));
    else {
      if (pid) {
        wait(0);
      }
      else {
        int fd = open(b, O_RDONLY, 0644);
        if(fd==-1){
          printf("error: no such file");
          exit(0);
        }
        int stdin = dup(0);
        dup2(fd,0);
        if ( execvp(command[0], command) == -1) {
          if (errno != 2) printf("Error: %s", strerror(errno));
          else printf("Error: Not a command");
        }
        dup2(stdin,0);
        exit(0);
      }
    }
  }
}


void parse(char * line) {
  printf("parse: %s\n", line);
  char del = 0;
  char tru = 0;
  int i = 0;
  char * a;
  char * b;
  
  if (strchr(line, ';')) {
    a = strsep(&line,";");
    parse(a);
    parse(line);
    return;
  }
  
  while ( line[i] && ! del) {
    if (line[i] == '<' || line[i] == '>' || line[i] == '|') del = line[i];
    i++;
  }
  
  a = strsep(&line, "><|");
  b = line;
  printf("a:%s\n",a);
  printf("b:%s\n",b);
  /*
  if (!del){
    exec(line,NULL);
    return;
  }

  if (del == ';'){
    exec(a,NULL);
    exec(b,NULL);
    return;
  }

  if (del == '>'){
    exec(a,b);
    return;
  }

  if (del == '<'){
    exec(a,b);
    return;
  }
  */
  if (del == '|'){
    redirect(a, ".temp");
    aredirect(b, ".temp");
    execlp("rm","rm",".temp",NULL);
    return;
  }
  
  else exec(a,b,del);
}
