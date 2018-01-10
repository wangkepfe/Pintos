// synchlist.h 
//	Data structures for synchronized access to a list.
//
//	Implemented by surrounding the List abstraction
//	with synchronization routines.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.
//
// modified by Vlad Jahundovics for Pintos (translation from C++ to C)


#include "copyright.h"
#include <list.h>
#include "threads/synch.h"

// The following class defines a "synchronized list" -- a list for which:
// these constraints hold:
//	1. Threads trying to remove an item from a list will
//	wait until the list has an element on it.
//	2. One thread at a time can access list data structures

struct SynchList {
  struct list sl_list;
  struct lock sl_lock;
  struct condition sl_empty;
};

struct SL_element
{
  struct list_elem elem;
  void *item;
};

void sl_init(struct SynchList *sl);
void sl_destroy(struct SynchList *sl);
void sl_append(struct SynchList *sl, void *item);
void *sl_remove(struct SynchList *sl);
