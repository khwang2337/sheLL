#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

void runP() {
    char str[20];
    char* line = str;
    fgets(line,20,stdin);
    *strchr(line, '\n') = 0; 
    int i = 0;
    char *command[20];//array of strings
    while ((command[i] = strsep(&line, " "))) i++;
    printf("%d\n", execvp(command[0], command));
    return 0;
}