/* Reads from the file and checks consistency.
 * The buffer should all contain the same character!!
 */

#include <syscall.h>
#include <stdio.h>
#include "pfs.h"

char buffer[BIG];

int main(void)
{
  int bytes, i, j, inconsistency;
  int id, messages;
  
  messages = open("messages");
  
  for (i = 0; i < TIMES; ++i)
  {
    id = open("file.1");
    bytes = read(id, buffer, BIG);
    close(id);
    
    if (bytes != BIG)
    {
      write(messages, "Buffer not filled!\n", 19);
      continue;
    }
    /* now check for consistency */
    for (j = 1, inconsistency = 0; j < BIG; ++j)
    {
      if (buffer[0] != buffer[j])
      {
        /* Ooops, inconsistency */
	write(messages, "INCONSISTENCY.", 14);
	printf("INCONSISTENCY\n");
	inconsistency = 1;
	break; /* no need to check further */
      }
    }
    if (!inconsistency)
    {
      write(messages, "cool\n", 5);
    }
  }
  close(messages);
  exit(0);
}
