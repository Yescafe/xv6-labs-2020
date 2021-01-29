#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
  char *args[MAXARG];
  char arg[512];
  char ch;
  int argn, argi;

  for (int i = 0; i < argc; ++i) {
    args[i] = (char *)malloc((strlen(argv[i]) + 1) * sizeof(char));
    strcpy(args[i], argv[i]);
  }

  argn = argc;
  argi = 0;
  while (read(0, &ch, 1)) {
    switch (ch) {
    case ' ':
      if (argi != 0) {
        arg[argi] = '\0';
        args[argn] = (char *)malloc((argi + 1) * sizeof(char));
        strcpy(args[argn++], arg);
        argi = 0;
      }
      break;
    case '\n':
      arg[argi] = '\0';
      args[argn] = (char *)malloc((argi + 1) * sizeof(char));
      strcpy(args[argn++], arg);
      args[argn] = 0;
      if (fork() == 0) {   // child
        exec(argv[1], args + 1);
      } else {            // parent
        wait(0);
      }

      for (int i = argc; args[i] != 0; ++i) {
        free(args[i]);
      }
      argn = argc;
      argi = 0;
      break;
    default:
      arg[argi++] = ch;
    }
  }
  // arg[argi] = '\0';
  // args[argn] = (char *)malloc((argi + 1) * sizeof(char));
  // strcpy(args[argn], arg);
  // if (fork() == 0) {   // child
  //   exec(argv[1], args + 1);
  // } else {            // parent
  //   wait(0);
  // }

  for (int i = 0; i < argc; ++i) {
    free(args[i]);
  }

  exit(0);
}
