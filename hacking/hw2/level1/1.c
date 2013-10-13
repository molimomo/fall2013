#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define HASH "ab8e63c8bbe4ef68e6bc718b90720d02"

int main(int argc, char *argv[]) 
{
  FILE *f;
  char buf[33];
  int i;

  
  f = popen("/bin/cat ~/.secret | /usr/bin/md5sum", "r"); 

  if ((i = fread(buf, 1, 32, f)) == 0) {
    perror("fread");
    return 1;
  }
  buf[i] = '\0';

  if (!strcmp(buf, HASH)) {
    execl("/bin/sh", "/bin/sh", (void *)NULL);
  }
  
  fprintf(stderr, "Wrong password\n");
  return 0;
}
  
  
