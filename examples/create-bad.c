/* The following program should be killed by the kernel because the
   string sent with create starts at one page (the BSS page) and ends
   outside of it in a page that is not allocated.

   If user memory accesses are not handled correctly by the kernel,
   then pintos will crash with a page fault exception.
   
   Author: Mattias Eriksson
*/

#include <syscall.h>
#include <stdio.h>
#include <string.h>

#define PGS 4096 /* page size */
#define PMASK 0xfffff000

static char inbss;

int
main (void)
{
  char * bss_page = (char*) ((int)(&inbss) & PMASK);
  printf("inbss and bss_page: %p, %p\n", &inbss, bss_page);
  printf("This program should be killed by the kernel.\n");

  memset ( bss_page, 'a', PGS );
  create (bss_page+PGS-5, 1024);
  printf("ERROR: this point should not be reached\n");
  
  exit(0);
}
