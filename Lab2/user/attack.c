#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

#define DATASIZE (8*4096)

int
main(int argc, char *argv[])
{
  // Your code here.
  char *data = sbrk(DATASIZE);

  printf("%s\n", data + 32);

  exit(1);
}
