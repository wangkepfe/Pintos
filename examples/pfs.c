#include "syscall.h"

int main(void)
{
  int i;
  int pid[5];
  
  create("file.1", 50000);
  create("messages", 5000);
  
  pid[0] = exec("pfs_writer a z");
  pid[1] = exec("pfs_writer A Z");
  pid[2] = exec("pfs_reader");
  pid[3] = exec("pfs_reader");
  pid[4] = exec("pfs_reader");
  
  for (i = 0; i < 5; i++)
  {
    wait(pid[i]);
  }
  exit(0);
}
