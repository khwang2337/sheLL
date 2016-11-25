#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main() {
  while (1){
    printf("sheLL $ ");
    char str[100] = "";
    char *line = str;
    fgets(line,sizeof(line,stdin));
    line[strlen(line)-1] = 0;
    parse(line);
  }
  return 0;
}
