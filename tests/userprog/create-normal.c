/* Creates an ordinary empty file. */

#include "tests/lib.h"
#include "tests/main.h"

void
test_main (void)
{
  //msg("%d\n",create ("quux.dat", 0));
  CHECK (create ("quux.dat", 0), "create quux.dat");
}
