// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.
//
// Modified by Viacheslav Izosimov
//   - transition from C++ to C (from Nachos to Pintos)


//#include "copyright.h"
//#include "system.h"
#include "threads/boundedbuffer.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "tests/threads/tests.h"
#include <stdio.h>
#include <string.h>

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void SimpleThread(void *);

void
SimpleThread(void * which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
		printf("*** thread %d looped %d times\n", (int)which, num);
		thread_yield();
//        currentThread->Yield();
    }
}

//----------------------------------------------------------------------
// ThreadTest
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
SimpleThreadTest(void)
{
//    DEBUG('t', "Entering SimpleTest");

//    Thread *t = new Thread("forked thread");
	char *t_name = "forked thread";
	printf("Entering SimpleTest");

	thread_create(t_name, PRI_MIN, SimpleThread, (void *)1);

//    t->Fork(SimpleThread, 1);
    SimpleThread((void *)0);
}
