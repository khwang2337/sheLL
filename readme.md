##"sheLL" shell by Ricky Wu, Kevin Hwang, Ruochong Wu
**Features:**
1. Forks and executes commands
2. Can execute multiple commands in one line using ';'
3. Redirects stdin/stdout using >,<
4. Piping using '|'
5. Chain multiple redirects and pipes


**Bugs**
1. Certain combinations of piping and redirections produce unexpected behavior (ex. ls>foo>boo)


**Files and function headers**
1. main.c -- driver file
  - int main() -- driver function, handles input, some basic processing (removing newline) and passing it on to functions in rpm.c.
2. rpm.c -- main function file
  - char * trimspace(char * ) -- removes whitespace before and after a line.
  - void parse(char * ) -- takes a single line of command from main, recursively separates line of command based on delimiter. passes separated commands (two at a time) to exec along with delimiter. Also handles piping by chaining redirect and aredirect
  - void exec(char * a, char * b, char del) -- recognizes special cases of exit and cd and executes accordingly. separates the line of the command by spaces to be used by execvp. Based on delimiter dups stdin/stdout appropriately and uses execvp to execute commands.
  - void redirect(char * line, char * b) -- originally intended to be used to handle >, downgraded to helper function for piping after original functionality integrated in exec
  - void aredirect(char * line, char * b) -- originally intended to be used to handle <, downgraded to helper function for piping after original functionality integrated into exec
