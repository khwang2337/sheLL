#include "rpm.c"

int main() {
  int clear; //define int used in clearing
  char str[100] = ""; //define string
  char *line = str; //define pointer
  
  while (1738) { //run indefinitely
    getcwd(str, 100); //get path
    printf("\nsheLL %s $ ", str); //print sheLL 
    
    if ( fgets(line, 100, stdin) == 0 ) printf("Error: %s", strerror(errno)); //takes input from stdin

    if ( line[strlen(line) - 1] != '\n' ) { //checks for out of bounds
      printf("Error: Out of bounds; character limit is 99\n"); 
      while ( (clear = getchar()) != '\n' && clear != EOF); //clears stdin
    }
    else {
        line[ strlen(line)-1 ] = 0; //replaces newline
        parse(line); //parses input
    }
  }
  return 0;
}