#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void memdump(char *fmt, char *data, int len);

int
main(int argc, char *argv[])
{
  if(argc == 1){
    printf("Example 1:\n");
    int a[2] = { 61810, 2025 };
    memdump("ii", (char*) a, sizeof(a));

    printf("Example 2:\n");
    char *t = "a string";
    memdump("S", t, strlen(t));

    printf("Example 3:\n");
    char *s = "another";
    memdump("s", (char *) &s, sizeof(char *));

    struct sss {
      char *ptr;
      int num1;
      short num2;
      char byte;
      char bytes[8];
    } example;

    example.ptr = "hello";
    example.num1 = 1819438967;
    example.num2 = 100;
    example.byte = 'z';
    strcpy(example.bytes, "xyzzy");

    printf("Example 4:\n");
    memdump("pihcS", (char*) &example, sizeof(example));

    printf("Example 5:\n");
    memdump("sccccc", (char*) &example, sizeof(example));
  } else if(argc == 2){
    // format in argv[1], up to 512 bytes of data from standard input.
    char data[512];
    int n = 0;
    memset(data, '\0', sizeof(data));
    while(n < sizeof(data)){
      int nn = read(0, data + n, sizeof(data) - n);
      if(nn <= 0)
        break;
      n += nn;
    }
    memdump(argv[1], data, strlen(data));
  } else {
    fprintf(2, "Usage: memdump [format]\n");
    exit(-1);
  }
  exit(0);
}

void
memdump(char *fmt, char *data, int len)
{
  // Your code here.
  int index = 0;
  for (char *tmp = fmt; *tmp; tmp++) {
    switch(*tmp) {
    case 'i':
      if (len - index < 4) {
        fprintf(2, "memdump: not enough data for 'i'\n");
        exit(-1);
      }
      printf("%d\n", *((int*) (data + index)));
      index += 4;
      break;
    case 'p':
      if (len - index < 8) {
        fprintf(2, "memdump: not enough data for 'p'\n");
        exit(-1);
      }
      printf("%lx\n", *((unsigned long*) (data + index)));
      index += 8;
      break;
    case 'h':
      if (len - index < 2) {
        fprintf(2, "memdump: not enough data for 'h'\n");
        exit(-1);
      }
      printf("%d\n", (int) *((short*) (data + index)));
      index += 2;
      break;
    case 'c':
      if (len - index < 1) {
        fprintf(2, "memdump: not enough data for 'c'\n");
        exit(-1);
      }
      printf("%c\n", *((char*) (data + index)));
      index += 1;
      break;
    case 's':
      if (len - index < 8) {
        fprintf(2, "memdump: not enough data for 's'\n");
        exit(-1);
      }
      printf("%s\n", *((char**) (data + index)));
      index += 8;
      break;
    case 'S':
      for (char *ptr = data + index; *ptr && ptr < data + len; ptr++) {
        printf("%c", *ptr);
      }
      printf("\n");
      return;
      break;
    default:
      fprintf(2, "Unexpected character: %c\n", *tmp);
      exit(-1);
      break;
    }
  }
}
