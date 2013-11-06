#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char *argv[])
{
  int pipefd[2];
  pid_t pid;
  int fd;
  int in;
  char c;
  int status;

  if (argv[1] == NULL) {  //Checks for command line argument
    fprintf(stderr, "Please specify the file to verify\n");
    return 1;
  }
   
  if ((in = open(argv[1], O_RDONLY)) < 0) { // opens given file into 'in' fd
    perror("open");
    return 2;
  }

  if ((fd = open("/dev/null", O_RDWR)) < 0) {  // opens dev/null into 'fd' fd
    perror("open");
    return 5;
  }
  
  if (pipe(pipefd) < 0) { //opens a pipe 
    perror("pipe");
    return 3;
  }

  if ((pid = fork()) < 0) { //forks this process!
    perror("fork");
    return 4;
  }
  

  if (pid == 0) {
    dup2(pipefd[0], 0);
    dup2(fd, 1); 
    dup2(fd, 2);  

    close(pipefd[1]);
    close(fd);
    close(in); 
    printf("\n I am the child! \n");    
    execlp("tidy", "tidy", "-asxml", (char *)0);
    printf("\n I am still the child! \n"); 
    perror("execlp");
    return 5;
  }

  while (read(in, &c, 1)) {
    write(pipefd[1], &c, 1);
  }

  close(pipefd[1]);
  close(pipefd[0]);
  close(fd);
  close(in);

  waitpid(pid, &status, 0);
  
  switch(WEXITSTATUS(status)) {
  case 0:
    printf("OK!\n");
    break;
  case 1:
    printf("Your file is not completely compliant\n");
    break;
  case 2:
    printf("Your file contains errors\n");
    break;
  default:
    printf("I can't tell if your file is XHTML-compliant\n");
  }

  return 0;
}
  
  
  
  
