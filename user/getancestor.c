#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

void
test_getancestor(int level)
{
  int apid = getancestor(level);
  if (apid >= 0) {
    printf("Ancestor level %d: PID %d\n", level, apid);
  } else {
    printf("Ancestor level %d: No such ancestor\n", level);
  }
}

int
main(int argc, char *argv[])
{
  int level, pid;

  if (argc != 2) {
    printf("Usage: %s <number_of_ancestors>\n", argv[0]);
    exit(1);
  }

  level = atoi(argv[1]);

  // Chain the forks to create a hierarchy
  for (int i = 0; i < 2; i++) {
    pid = fork();
    if (pid > 0) {
      // Parent process waits for the child to finish
      wait(0);
      exit(0); // Exit once child is done to maintain the hierarchy
    } else if (pid == 0) {
      // Child process continues the loop
      continue;
    } else {
      // Fork failed
      printf("Fork fallado");
      exit(1);
    }
  }

  // In the last child process, test getancestor
  if (pid == 0) {
    printf("ID del proceso actual: %d\n", getpid());
    test_getancestor(level);
    exit(0);
  }

  return 0;
}
