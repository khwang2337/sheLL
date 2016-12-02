##"sheLL" shell by Ricky Wu, Kevin Hwang, Ruochong Wu
**Features:**
1. Forks and executes commands

2. Can execute multiple commands in one line using ';'

3. Redirects stdin/stdout using >,<

4. Redirects stdin/stdout appending to file using >>.

5. Redirects stderr using 2> or 2>>

6. Redirects both stdout/stderr using &> or &>>

7. Piping using '|'

8. Chain multiple redirects and pipes


**Bugs**

1. Certain combinations of piping and redirections produce unexpected behavior (ex. ls>foo>boo)  (Fixed? Hard to test all combinations.)


**Files and function headers**

1. main.c -- driver file
  - int main() -- driver function, handles input, some basic processing (removing newline) and passing it on to functions in rpm.c.

2. rpm.c -- main function file
  - char * trimspace(char * ) -- removes whitespace before and after a line.
  - void redirect(char * command[], char * b, char app) -- Executes command with delimiter '>'. Takes parsed command in array form and redirects stdout using dup2 to file b, also serves as helper function for exec and piper. If app = 1 b is opened with the O_APPEND flag.
  - void aredirect(char * command[], char * b) -- Executes command with delimiter '<'. Takes parsed command in array form and redirects file b into stdin using dup2, also serves as helper function for exec and piper.
  - void piper(char * command[], char * b) -- Executes command with delimiter '|'. Takes 1st command in array form, 2nd command in string form. Executes first command redirecting stdout to a temp file, and executes second command with stdin coming from the same temp file.
  - void inerredirect(char * command[], char * b, char app) -- Executes command with delimiter '&>'. Takes parsed command in array form and redirects stdout and stderr using dup2 to file b. If app = 1 opens b with the O_APPEND flag.
  - void erredirect(char * command[], char * b, char app) -- Executes command with delimiter '2>', '2>>'. Takes parsed command in array form and redirects stderr using dup2 to file b. If app = 1 opens b with the O_APPEND flag.
  - void parse(char * ) -- takes a single line of command from main, recursively separates line of command based on delimiter. passes separated commands (two at a time) to exec along with delimiter.
  - void exec(char * a, char * b, char del) -- recognizes special cases of exit and cd and executes accordingly. separates the line of the command by spaces to be used by execvp. If no delimter, executes using execvp. If there is a delimiter calls the appropriate helper function to execute.
