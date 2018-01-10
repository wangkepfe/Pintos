/* Write on the disk.
 * Each time the buffer is filled with same character.
 * Different character every time!
 */

#include <syscall.h>
#include <stdio.h>
#include <string.h>
#include "pfs.h"

char buffer[BIG];

int main(int argc, char* argv[])
{
  int i,j;
  char c;
  int id;
  int write_count;
  char start;
  char end;

  if (argc != 3 || strlen(argv[1]) != 1 || strlen(argv[2]) != 1)
    exit(1);

  start = argv[1][0];
  end   = argv[2][0];

  //printf("start is %c, end is %c\n",start,end);

  for (i = 0; i < TIMES / (end - start + 1) + 1; ++i)
  {
    for (c = start; c <= end; ++c)
    {
      for (j = 0; j < BIG; j++)
        buffer[j] = c;

      id = open("file.1");
      //printf("open function return value is %d\n",id);

      write_count = write(id, buffer, BIG);
      //printf("write function return value is %d\n",write_count);

      if ( write_count != BIG )
      {
        printf("TEST ERROR: write() wrote only %d bytes out of %d bytes\n",
               write_count, BIG);
      }
      close(id);
    }
  }
  exit(0);
}
