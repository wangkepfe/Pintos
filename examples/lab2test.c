/* I cleaned this file up a bit, but it is still not pretty. -- ME */
#include <stdio.h>
#include <string.h>
#include <syscall.h>
#include <stdarg.h>

int main(void)
{  
  char *descr = "This is test program v0.1 that tests your implementation basic system calls\n";
  char *test1 = "The first test is to create three files.\n";
  char *test2 = "Now lets write some data to the files. Write some binary data to some files.\n";
  char *test3 = "Test string that is written to a file.\n";
  char *test4 = "Test to read back data from files.\n";
  char *test5 = "Test to read from and write to bad file handles.\n";
  char *test6 = "Get data from console. Please, write something (10 characters).\n";
  char *test7 = "Test to open a file that does not exist.\n";
  char binarydata[10];
  char sbuf[50];
  int file[3];
  int num_bytes_read, i;

  binarydata[0] = 1;
  binarydata[1] = 2;
  binarydata[2] = 3;
  binarydata[3] = 4;
  binarydata[4] = 5;
  binarydata[5] = 0;
  binarydata[6] = 1;
  binarydata[7] = 2;
  binarydata[8] = 3;
  binarydata[9] = 4;

  write(STDOUT_FILENO, descr, strlen(descr));
  write(STDOUT_FILENO, test1, strlen(test1));
  if (!create("test0", strlen(test3))) {
    printf("Could not create test0\n");
    halt();
  }
  if (!create("test1", 1024)){
    printf("Could not create test1\n");
    halt();
  }
  if (!create("test2", 1024)){
    printf("Could not create test1\n");
    halt();
  }
	
  for(i = 0; i < 2; i++){
    snprintf(sbuf, 50, "test%d", i);
    file[i] = open(sbuf);
    if(file[i] > 1){
    }
    else{
      printf("Could not open %s\n", sbuf);
      halt();
    }
  }
	
  write(STDOUT_FILENO, test6, strlen(test6));
  num_bytes_read = read(STDIN_FILENO, sbuf, 10);
  if(num_bytes_read != 10){
    printf("Did not read 10 characters from the console.\n");
    halt();
  }
	
  write(STDOUT_FILENO, test2, strlen(test2));
  write(file[0], test3, strlen(test3));
  write(file[1], binarydata, 10);
	
  write(STDOUT_FILENO, test4, strlen(test4));
  close(file[0]);
  file[0] = open("test0");
  num_bytes_read = read(file[0], sbuf, 50);
  if(num_bytes_read != (int)strlen(test3)){
    printf("Could not read back from test0 (%u),  %u characters read instead!\n", (unsigned int)strlen(test3), num_bytes_read);
    halt();
  }
  
  num_bytes_read = read(file[1], sbuf, 10);
  if(num_bytes_read != 10){
    printf("Could not read back binary data from test1\n");
    halt();
  }
  if(!memcmp(sbuf, binarydata, 10)) {
    printf("Data read back from test1 does not match what was written\n");
    halt();
  }
  close(file[1]);
	
  write(STDOUT_FILENO, test5, strlen(test5));
  write(87, "hej", 3);
  num_bytes_read = read(1006, sbuf, 3);
  if(num_bytes_read != -1){
    printf("You should catch that I tried to read from file 1006\n");
  }
	
  write(STDOUT_FILENO, test7, strlen(test7));
  file[2] = open("fdsfa");
  if(file[2] > 1){
    printf(sbuf, "Got file handle when opening non-existing file\n");
    write(STDOUT_FILENO, sbuf, strlen(sbuf));
    halt();
  }

  printf("Test passed\n");
  halt();
}
