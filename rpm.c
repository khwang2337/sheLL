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

char * trimspace(char * a) {
  if (a) {
    int count = 0;
    char * ret = 0;
    while (a[count] == 32){
      count++;
    }
    ret = a+count;
    count = strlen(a)-1;
    while (a[count] == 32){
      count--;
    }
    a[count+1]=0;
    return ret;
  }
  else return a;
}

void redirect(char * command[], char *b){
  int fd = open(b, O_CREAT|O_RDWR|O_TRUNC, 0644);
  int stdout = dup(1);
  dup2(fd,1);
  if ( execvp(command[0], command) == -1) {
    if (errno != 2) printf("Error: %s", strerror(errno));
    else printf("Error: Not a command");
  }
  dup2(stdout,1);
}

void aredirect(char * command[], char *b){
  int fd = open(b, O_RDONLY, 0644);
  if( fd == -1){
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
}

void exec(char * a, char * b, char del) {
    int pid, i = 0;
    char* Acommand[C_SIZE]; 
    
    printf("exec a:%s\n",a);
    printf("exec b:%s\n",b);
    printf("del:%c\n", del);
    
    while (Acommand[i] = strsep(&a, " ")) i++;
    printf("%s\n", Acommand[0]);
    if (! strcmp(Acommand[0],"cd") ) {
      if (chdir(Acommand[1]) == -1) printf("Error: %s", strerror(errno));
    }
    else if (! strcmp(Acommand[0],"exit") ) exit(0);
    else {
      if ( (pid = fork()) == -1) printf("Error: %s", strerror(errno));
      else {
        if (pid) {
          wait(0);
        }
        else {
          if (! del) {
            if ( execvp(Acommand[0], Acommand) == -1) {
              if (errno != 2) printf("Error: %s", strerror(errno));
              else printf("Error: Not a command");
            }
          }
          else {
            if (del == '>') redirect(Acommand,b);
            if (del == '<') aredirect(Acommand,b);
            if (del == '|') {
              int pid2;
              i = 0;
              char * Bcommand[C_SIZE];
              while (Bcommand[i] = strsep(&b, " ")) i++;
              
              i = 2;
              while (i--) {
                pid2 = fork();
                if (pid2) {
                  wait(0);
                }
                else {
                  if (i == 1) redirect(Acommand,".temp");
                  else aredirect(Bcommand,".temp");
                  exit(0);
                }
              }
              
              execlp("rm", "rm", ".temp", NULL);
            }
          }
          exit(0);
        }
      }
    }
}

void parse(char * line) {
  printf("parse:%s\n", line);
  char del = 0, i = 0;
  char * a, * b;
  
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
  
  a = trimspace(strsep(&line, "><|"));
  b = trimspace(line);
  printf("parse a:%s\n",a);
  printf("parse b:%s\n",b);
  
/*  
  if (del == '|') {
    printf("del %c\n", del);
    char* command[C_SIZE]; 
    
    i = 0;
    while (command[i] = strsep(&a, " ")) i++;
    
    int pid = fork();
    if (pid) wait(0);
    else {
      redirect(command, ".temp");
      
      i = 0;
      while (command[i] = strsep(&b, " ")) i++;
      
      aredirect(command, ".temp");
      execlp("rm","rm",".temp",NULL);
      exit(0);
    }
    
    return;
  }
  */
  exec(a,b,del);
}
