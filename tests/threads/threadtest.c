// threadtest.c 
//	Simple test case for the threads assignment.
//
//	Create seven threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustrate the inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.
//
// Modified by Levon Saldamli.
// Modified by Andrzej Bednarski:
//  - added #ifdef (do not require modification of Makefile)
// Modified by Vlad Jahundovics:
//  - transition from C++ to C (from Nachos to Pintos)

//#ifdef THREADS

//#include "copyright.h"
//#include "system.h"
//#include "synch.h"
//#include "boundedbuffer.h"
#include "threads/boundedbuffer.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "tests/threads/tests.h"
#include <stdio.h>
#include <string.h>

#define NBR_OF_PROD 50
//const int nbr_of_prod = 50;
#define NBR_OF_CON 50
//const int nbr_of_con = 50;
const int prod_switch = 30;
const int con_switch = 20;

char * prod_name[NBR_OF_PROD];
char * con_name[NBR_OF_CON];

char * msg_array[NBR_OF_PROD];

char * received_msg_array[NBR_OF_PROD];
int received_msg_pos[NBR_OF_PROD];


struct bounded_buffer bounded_buffer[2];

struct lock readlock[2];

struct lock start_lock;
struct condition start_cond;

int started_threads;

/*
class Data
{
public:
  Data(char ch, char* s, int i) {
    c = ch;
    sender = s;
    sender_index = i;
  }
  char c;
  char *sender;
  int sender_index;
};
*/

struct Data
{
  char c;
  char *sender;
  int sender_index;
};

void data_init(struct Data *data, char ch, char* s, int i);
void WaitForStart(void);
void Producer(void *index);
void Consumer(void *index);

void data_init(struct Data *data, char ch, char* s, int i) 
{
  data->c = ch;
  data->sender = s;
  data->sender_index = i;
}

void
WaitForStart(void) {
  lock_acquire(&start_lock);
  printf("%s is waiting for start signal\n", thread_name());
  cond_wait(&start_cond,&start_lock);
  printf("%s is starting\n", thread_name());
  started_threads++;
  lock_release(&start_lock);
}

void
Producer(void *index)
{
  char *sender_name;
  size_t sender_name_length;
  WaitForStart();
  int buf=0;
  if ((int) index >= prod_switch)
    buf=1;

  char *msg = msg_array[(int) index];
  void *p;

  for (; *msg != '\0'; ++msg) {
    p = malloc(sizeof(struct Data));
    sender_name_length = strlen(thread_name())+1;
    sender_name = calloc(sizeof(char), sender_name_length);
    strlcpy(sender_name,thread_name(), sender_name_length);
    data_init(p, *msg, sender_name, (int) index);
    bb_write(&bounded_buffer[buf],(int) p);
    thread_yield();
  }
  p = malloc(sizeof(struct Data));
  sender_name_length = strlen(thread_name())+1;
  sender_name = calloc(sizeof(char), sender_name_length);
  strlcpy(sender_name,thread_name(), sender_name_length);
  data_init(p, 0, sender_name, (int) index);
  bb_write(&bounded_buffer[buf],(int) p);
  printf("%s has finished sending.\n", thread_name());
}

void
Consumer(void *index)
{
  WaitForStart();

  int buf=0;
  if ((int) index >= con_switch)
    buf=1;

  while (true) {

    lock_acquire(&readlock[buf]);
    struct Data *data = (struct Data*) bb_read(&bounded_buffer[buf]);
    int i=data->sender_index;
    received_msg_array[i][received_msg_pos[i]] = data->c;
    received_msg_pos[i]++;
    lock_release(&readlock[buf]);

    if (data->c != 0);
      /*      DEBUG('c', "%s received from %s: %c\n", 
	    currentThread->getName(), 
	    data->sender,
	    data->c);*/
    else
      printf("\n%s: %s's total message was: \n\"%s\"\n", 
	     thread_name(),
	     data->sender,
	     received_msg_array[i]);
    free(data->sender);
    free(data);
    thread_yield();
  }
}



//----------------------------------------------------------------------
// ThreadTest
// 	Creates three consumers and three producers. The producers
// 	write different messages to the buffer. The main thread also
// 	calls Consumer, resulting in four consumers.
//----------------------------------------------------------------------

void
ThreadTest(void)
{
  //DEBUG('t', "Entering SimpleTest\n");
    const int nmsg = 5;
    char *msg[nmsg];
    msg[0] = "Computer, compute to the last digit the value of pi!";
    msg[1] = "What is now proved was once only imagined.";
    msg[2] = "Insufficient facts always invites danger, Captain.";
    msg[3] = "The Federation's gone; the Borg is everywhere!";
    msg[4] = "Live long and prosper, Spock.";

    //    bounded_buffer[0] = new bounded_buffer(5);
    //    bounded_buffer[1] = new bounded_buffer(5);

    printf("ThreadTest has just started! It's thread name is %s.\n", thread_name());

    bb_init(&bounded_buffer[0],5);
    bb_init(&bounded_buffer[1],5);
    lock_init(&readlock[0]);
    lock_init(&readlock[1]);
    //    readlock[0] = new Lock("Read lock 0");
    //    readlock[1] = new Lock("Read lock 1");

    lock_init(&start_lock);
    //    start_lock = new Lock("Start lock");
    cond_init(&start_cond);
    //    start_cond = new Condition("Start cond");
    started_threads = 0;

    char pname[] = "Producer";
    char cname[] = "Consumer";
    int i;

    for (i=0;i < NBR_OF_PROD; i++) {
      //      char *str = new char[strlen(pname)+4];
      char *str = (char *) calloc(sizeof(char), strlen(pname)+4);
      snprintf(str,strlen(pname)+4,"%s %02d", pname, i);

      prod_name[i] = str;
      msg_array[i] = msg[i%nmsg];
      received_msg_array[i] = (char *) calloc(sizeof(char),strlen(msg_array[i])+1);
      //received_msg_array[i] = new char[strlen(msg_array[i])+1];
      received_msg_pos[i] = 0;
      //      printf("Creating thread with the name: %s\n",str);
      thread_create(str, PRI_MIN, Producer, (void *) i);
      free(str);
      //      Thread *t = new Thread(str);
      //      t->Fork(Producer, i);
    }
    for (i=0;i < NBR_OF_CON; i++) {
      char *str = (char *) calloc(sizeof(char), strlen(cname)+4);
      //char *str = new char[strlen(cname)+4];
      snprintf(str, strlen(cname)+4,"%s %02d", cname, i);
      //      printf("Creating thread with the name: %s\n",str);
      thread_create(str, PRI_MIN, Consumer, (void *) i);
      free(str);
      //Thread *t = new Thread(str);
      //t->Fork(Consumer, i);
    }

    thread_yield();

    lock_acquire(&start_lock);
    while (started_threads < (NBR_OF_PROD + NBR_OF_CON) ) {
      printf("\n\n%s : All threads haven't started. Broadcasting start signal to all threads\n\n", thread_name());
      cond_broadcast(&start_cond, &start_lock);
      //      start_cond->Broadcast(start_lock);
      lock_release(&start_lock);
      thread_yield();
      lock_acquire(&start_lock);
    }
    printf("\n\nAll threads have started. Finishing %s\n\n", thread_name());
}

//#endif // THREADS
