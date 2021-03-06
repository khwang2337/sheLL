#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
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

//> and >> redirect stdout
void redirect(char * command[], char *b, char app){
  int fd;
  if (app) fd = open(b, O_CREAT|O_RDWR|O_APPEND, 0644);
  else fd = open(b, O_CREAT|O_RDWR|O_TRUNC, 0644);
  int stdout = dup(1);
  dup2(fd,1);
  if ( execvp(command[0], command) == -1) {
    if (errno != 2) printf("Error: %s", strerror(errno));
    else printf("Error: Not a command");
  }
  dup2(stdout,1);
}

//< redirect in
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

//| pipe
void piper(char * command[], char * b) {
  int pid, i = 0;
  char * command2[C_SIZE];
  while ((command2[i] = strsep(&b, " "))) i++;

  i = 2;
  while (i--) {
    pid = fork();
    if (pid) {
      wait(0);
    }
    else {
      if (i) redirect(command,".temp",0);
      else aredirect(command2,".temp");
      exit(0);
    }
  }
  execlp("rm", "rm", ".temp", NULL);
}

//&> and &>>  redirect stdout and stderr
void inerredirect(char * command[], char *b, char app){
  int fd;
  if (app) fd = open(b, O_CREAT|O_RDWR|O_APPEND, 0644);
  else fd = open(b, O_CREAT|O_RDWR|O_TRUNC, 0644);
  int stdout = dup(1);
  int stderr = dup(2);
  dup2(fd,1);
  dup2(fd,2);
  if ( execvp(command[0], command) == -1) {
      if (errno != 2) printf("Error: %s", strerror(errno));
      else printf("Error: Not a command");
  }
  dup2(stdout,1);
  dup2(stderr,1);
}

//2> and 2>> redirect stderr
void erredirect(char * command[], char *b, char app) {
  int fd;
  if (app) fd = open(b, O_CREAT|O_RDWR|O_APPEND, 0644);
  else fd = open(b, O_CREAT|O_RDWR|O_TRUNC, 0644);
  int stderr = dup(2);
  dup2(fd,2);
  if ( execvp(command[0], command) == -1) {
    if (errno != 2) printf("Error: %s", strerror(errno));
    else printf("Error: Not a command");
  }
  dup2(stderr,1);
}

void exec(char * a, char * b, char del) {
    int pid, i = 0;
    char* command[C_SIZE]; 
    
    while ((command[i] = strsep(&a, " "))) i++;

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
            if (del == '|') piper(command,b); //|
            if (del == '<') aredirect(command,b); //<
            if (del == '>') redirect(command,b,0); //>
            if (del == 36) redirect(command,b,1); //>>
            if (del == 40) erredirect(command,b,0); //2>
            if (del == 48) erredirect(command,b,1); //2>>
            if (del == 64) inerredirect(command,b,0); //&>
            if (del == 101) inerredirect(command,b,1); //&>>
          }
          exit(0);
        }
      }
    }
}

void parse(char * line) {
  char del = 0, i = 0;
  char * a, * b;
  
  if (strchr(line, ';')) {
    a = strsep(&line,";");
    parse(a);
    parse(line);
    return;
  }

  if (strstr(line, "&>>")) {
    del = 101;
    a = trimspace(strsep(&line,"&"));
    strsep(&line, ">");
    strsep(&line, ">");
  }
  else if (strstr(line, "&>")) {
    del = 64;
    a = trimspace(strsep(&line,"&"));
    strsep(&line, ">");
  }
  else if (strstr(line, "2>>")) {
    del = 48;
    a = trimspace(strsep(&line,"2"));
    strsep(&line, ">");
    strsep(&line, ">");
  }
  else if (strstr(line, "2>")) {
    del = 40;
    a = trimspace(strsep(&line, "2"));
    strsep(&line, ">");
  }
  else if (strstr(line, ">>")) {
    del = 36;
    a = trimspace(strsep(&line,">"));
    strsep(&line, ">");
  }
  else {
    while ( line[i] && ! del) {
      if (line[i] == '<' || line[i] == '>' || line[i] == '|') del = line[i];
      i++;
    }
  
    a = trimspace(strsep(&line, "><|"));
  }
  
  b = trimspace(line);
  
  exec(a,b,del);
}
