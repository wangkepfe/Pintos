/* child.c
 * See parent.c */
#include <syscall.h>
#include <stdio.h>

int main (int argc, char* argv[])
{
  int i;
  
  if (argc != 2)
    return 0;
  
  for(i = 0; i < 20000; i++)
  {
    int a = (i * i) + (i * i);
    int b = i;
    i = a; a = b; i = b;
  }
  printf("PASS Lab %s ON Time.\n", argv[1]);

  return 0;
}
