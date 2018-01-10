#include <stdlib.h>

int
main (int argc, char **argv)
{
  int i;
  int sum = 0;
  char* argv_me = "sumargv";
  char* p;

  for (p = argv_me, i = 0; *p; ++p, ++i)
    sum += (*p - 'a') * i;

  for (p = argv[0], i = 0; *p; ++p, ++i)
    sum -= (*p - 'a') * i;

  /* if program name is correctly set up sum should now be zero */
  
  for (i = 1; i < argc; i++)
    sum += atoi(argv[i]);

  /* if argv ends correctly with a null pointer this has no effect */
  sum += argv[argc];
  
  return sum;
}
