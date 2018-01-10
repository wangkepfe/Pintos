/* Start a lot of processes and let them finish to test if we
 * eventually run out of process slots.
 * To run the test effectively, if you have a limit on number of
 * processes, lower the limit to 5 processes and run this test like
 *
 *  'longrun 5 400'
 *
 * It will fill (and if it works empty) your process table 400 times
 * :-)
 *
 * Note that some pintos tests requires at least 16 simultaneous
 * processes to work, so be sure to increase the limit before running
 * pintos tests.
 */

#include <syscall.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_SIMULTANEOUS 50
#define MAX_REPEAT 1000

int main(int argc, char* argv[])
{
  char cmd[15];
  int pid[MAX_SIMULTANEOUS];
  unsigned i = 0;
  unsigned j = 0;
  unsigned simul = 0;
  unsigned repeat = 0;

  if (argc != 3)
  {
    printf("Usage: %s simultaneous repeat\n"
           "Where 'simultaneous' is the number of simultaneous processes to \n"
           "start and wait for, and 'repeat' how many times this should be \n"
           "repeated.\n", argv[0]
      );
    return -1;
  }

  simul = atoi(argv[1]);
  repeat = atoi(argv[2]);

  if (simul > MAX_SIMULTANEOUS)
  {
    printf("This test program is compiled with a limitation to max %d \n"
           "simultaneos processes.\n", MAX_SIMULTANEOUS);
    return -1;
  }
  
  if (repeat > MAX_REPEAT)
  {
    printf("This test program is compiled with a limitation to max %d \n"
           "repetitions.\n", MAX_REPEAT);
    return -1;
  }
  
  printf("Will try to start a total of %d processes in groups of %d\n",
         simul * repeat, simul);
  
  for (j = 0; j < repeat; ++j)
  {
    for (i = 0; i < simul; ++i)
    {
      snprintf(cmd, 15, "dummy %i", j * simul + i);
      pid[i] = exec(cmd);
    }
    
    /* There will never be more than 'simul' processes running
     * simultaneously, but in total there will be a lot of processes
     * (repeat * simul). Just as you can expect in a real system.
     */
    for (i = 0; i < simul; ++i)
    {
      /* probably nothing left to wait for */
      if (pid[i] == -1)
      {
        printf("ERROR: Aborting test. Some execution(s) failed\n");
        return j+1;
      }
      wait(pid[i]);      
    }
  }
  return 0;
}
