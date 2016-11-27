#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

void exec(

void parse(char* line){
    int i = 0;
    char* command[100];
    while (line){
        command [i] = strsep(&line, " ");
        i++;
    }
    command[i] = NULL;
    printf("%d\n", execvp(command[0], command));
}
