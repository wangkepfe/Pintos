/* Creates an ordinary empty file. */

#include <syscall.h>
#include <stdio.h>
#include <string.h>

int
main (void)
{
  printf("return value is %d\n",create ("quux.dat", 0));
  //CHECK (create ("quux.dat", 0), "create quux.dat");
  exit(0);
}
