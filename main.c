#include "rpm.c"


int main() {
  while (1){
    printf("sheLL $ ");
    char str[100] = "";
    char *line = str;
    //If invalid input:
    if (fgets(line, 100, stdin) == 0)
        printf("Error: %s\n", strerror(errno));
    else if (line[strlen(line) - 1] != '\n')
        printf("Error: out of bounds!\nChar limit: 99");
    else {
        line[strlen(line)-1] = 0;//replaces \n
        parse(line);
    }
  }
  return 0;
}
