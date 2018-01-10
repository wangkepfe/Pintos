/* parent.c
 * A test program that calls itself recursively.
 * In the last step of the recursion child.c is started.
 * Do not use with large CHILDREN or DEPTH.
 */
#include <syscall.h>
#include <stdlib.h>
#include <stdio.h>

#define CHILDREN 4
#define DEPTH 3

int main(int argc, char* argv[])
{
  int i;
  int pid[CHILDREN];
  int depth = DEPTH - 1;
  char cmd[10];

  if (argc == 2)
    depth = atoi(argv[1]) - 1;
  
  for(i = 0; i < CHILDREN; i++)
  {
    if (depth)
      snprintf(cmd, 10, "parent %i", depth);
    else
      snprintf(cmd, 10, "child %i", i);

    printf("%s\n", cmd);
    pid[i] = exec(cmd);
  }
//  if (depth <= 1)
  {
    for(i = 0; i < CHILDREN; i++)
    {
      wait(pid[i]);
    }
  }
  exit(0);
}
