#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

int main(int argc, char *argv[]) {
  int pipefd[2];
  pid_t cpid1;
  pid_t cpid2;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s CMD1 CMD2\n", argv[0]);
    exit(1);
  }

  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  cpid1 = fork();
  if (cpid1 == 0) {
    dup2(pipefd[1], 1);
    close(pipefd[0]);
    close(pipefd[1]);
    execlp(argv[1], argv[1], NULL);
  } else {
    cpid2 = fork();
    if (cpid2 == 0) {
      dup2(pipefd[0], 0);
      close(pipefd[0]);
      close(pipefd[1]);
      execlp(argv[2], argv[2], NULL);
    } else {
      exit(0);
    }
  }
}
