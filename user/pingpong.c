#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char byte;
  int parent[2], child[2];
  pipe(parent);
  pipe(child);

  int pid = fork();
  if(pid == 0){   // child
    read(parent[0], &byte, 1);
    printf("%d: received ping\n", getpid());
    
    write(child[1], &byte, 1);
  } else if(pid > 0){  // parent
    write(parent[1], &byte, 1);

    read(child[0], &byte, 1);
    printf("%d: received pong\n", getpid());
  } else {
    fprintf(2, "fork error\n");
    exit(1);
  }
  
  exit(0);
}
