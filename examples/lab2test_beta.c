#include <stdio.h>
#include <string.h>
#include <syscall.h>
#include <stdarg.h>
#include "debug.h"

void fail(void);

int main(int argc UNUSED, char ** argv UNUSED)
{  
  char *test_string = "This is a string that is written to a file.\n";
  char byte_array[10] = {0,1,2,3,4,0,1,2,3,4};
  char sbuf[50];
  int file[3];
  int num_bytes_read, num_bytes_written;

  printf("\n");
  printf("*****************************************************************\n");
  printf("* This program tests your implementation of basic system calls. *\n");
  printf("*****************************************************************\n\n");

  printf("The first test is to create three files.\n");
  printf("(Make sure that you have deleted any test files from previous test runs.)\n");
  if (!create("test0", (int)strlen(test_string)) || !create("test1", 256) || !create("test2", 256)) {
    printf("Could not create the testfiles.\n");
    fail();
  }
	
  file[0] = open("test0");
  file[1] = open("test1");
  file[2] = open("test2");

  printf("Got file descriptors: %d, %d and %d\n", file[0], file[1], file[2]);

  if (file[0] < 0 || file[1] < 0 || file[2] < 0) {
    printf("Could not open the test files.\n");
    fail();
  }
	
  printf("\nTesting to read from console. Type 10 characters, please.\n");
  num_bytes_read = read(STDIN_FILENO, sbuf, 10);
  if (num_bytes_read != 10) {
    printf("Did not read 10 characters from the console.\n");
    fail();
  }
  printf("This is what you wrote: \"%s\"\n", sbuf);
	
  printf("\nTesting to write data to files.\n");
  write(file[0], test_string, strlen(test_string));
  write(file[1], byte_array, 10);
	
  printf("\nTesting to read data back from files.\n");
  /* Reopening to reset seek position. */
  close(file[0]);
  file[0] = open("test0");
  num_bytes_read = read(file[0], sbuf, 50);
  if (num_bytes_read != (int)strlen(test_string)) {
    printf("Could not read back from test0 (%d),  %d characters read instead!\n", (int)strlen(test_string), num_bytes_read);
    fail();
  }
  
  close(file[1]);
  file[1] = open("test1");
  num_bytes_read = read(file[1], sbuf, 10);
  if (num_bytes_read != 10) {
    printf("Could not read back binary data from test1\n");
    fail();
  }
  if (memcmp(sbuf, byte_array, 10) != 0) { /* memcmp returns 0 if the data are identical */
    printf("Data read back from test1 does not match what was written.\n");
    fail();
  }
  close(file[1]);
	
  printf("\nTesting write with a bad file descriptor.\n");
  num_bytes_written = write(file[1], "FOO", 3);
  if (num_bytes_written != -1) {
    printf("Writing to a closed file should not be possible.\n");
    fail();
  }

  printf("\nTesting read with a bad file descriptor.\n");
  /* Assuming here that 424242 is a bad file descriptor. */
  num_bytes_read = read(424242, sbuf, 3);
  if (num_bytes_read != -1) {
    printf("Writing to a bad fd should not work.\n");
    fail();
  }
	
  printf("\nTesting to open a non-existing file.\n");
  file[2] = open("this.file.does.not.exist!");
  if (file[2] > 1) {
    printf("Got file handle when opening non-existing file.\n");
    fail();
  }

  printf("\n");
  printf("*****************************************************************\n");
  printf("*                 Tests passed; halting machine.                *\n");
  printf("*****************************************************************\n\n");
  halt();
  return -1;
}

void fail(void)
{
  printf("\n");
  printf("*****************************************************************\n");
  printf("*                          TEST FAILED                          *\n");
  printf("*****************************************************************\n\n");
  halt();
}
