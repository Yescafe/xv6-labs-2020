#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define UP_BOUND 35

int
isPrime(int n)
{
  int a;
  for (a = 2; a < n; ++a) {
    if (n % a == 0)
      return 0;
  }
  return 1;
}

int
findNextPrime(int k)
{
  ++k;
  while (k <= 35) {
    if (isPrime(k)) return k;
    ++k;
  }
  return -1;
}

int
thread(int k)
{
  int writePrime = findNextPrime(k), readPrime;
  int p[2];
  pipe(p);

  if (writePrime < 0) {
    return 0;
  }

  int pid = fork();
  if (pid == 0) {             // child
    close(p[1]);
    read(p[0], &readPrime, 4);
    close(p[0]);
    printf("prime %d\n", readPrime);
    thread(readPrime);
  } else if (pid > 0) {       // parent
    close(p[0]);
    write(p[1], &writePrime, 4);
    close(p[1]);
    wait(0);
  } else {
    fprintf(2, "fork error\n");
    return 1;
  }

  return 0;
}

int
main(int argc, char *argv[])
{

  if (argc > 2) {
    fprintf(2, "Usage: primes");
    exit(1);
  }

  exit(thread(1));
}
