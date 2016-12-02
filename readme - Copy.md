https://linux.die.net/man/2/pipe

char * del[C_SIZE];
  int n = 0;
  int i = 0;
  char * cmd[C_SIZE];
  
  while ( line[i] ) {
    if (line[i] == '<' || line[i] == '>' || line[i] == '|' || line[i] == ';') del[n++] = line[i];
    i++;
  }
  
  i = 0;
  while (cmd[i] = strsep(&line, "><|;")) i++;
  printf("%s|%s\n", cmd[0], cmd[1]);
  int _i = 0;
  int _n = 0;
  while (!(_i==i)) {
    if (del[_n++] == ';') exec(cmd[_i]);
    _i++;
  }
  exec(cmd[_i]);