// synchlist.cc
//	Routines for synchronized access to a list.
//
//	Implemented by surrounding the List abstraction
//	with synchronization routines.
//
// 	Implemented in "monitor"-style -- surround each procedure with a
// 	lock acquire and release pair, using condition signal and wait for
// 	synchronization.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.
//
// modified by Vlad Jahundovics for Pintos (translation from C++ to C)


#include "copyright.h"
#include "synchlist.h"
#include "threads/malloc.h"

//----------------------------------------------------------------------
// SynchList::SynchList
//	Initialize the data structures needed for a 
//	synchronized list, empty to start with.
//	Elements can now be added to the list.
//----------------------------------------------------------------------

void sl_init(struct SynchList *sl)
{
  list_init(&sl->sl_list);
  lock_init(&sl->sl_lock);
  cond_init(&sl->sl_empty);
}


//----------------------------------------------------------------------
// SynchList::~SynchList
//	Remove and de-allocate all elements of the synchronized list
//----------------------------------------------------------------------

void sl_destroy(struct SynchList *sl)
{
  struct list_elem *e;
  struct SL_element *sl_elem;
  while(!list_empty(&sl->sl_list)){
    e = list_pop_front(&sl->sl_list);
    sl_elem = list_entry(e, struct SL_element, elem);
    free(sl_elem);
  }
}


//----------------------------------------------------------------------
// SynchList::Append
//      Append an "item" to the end of the list.  Wake up anyone
//	waiting for an element to be appended.
//
//	"item" is the thing to put on the list, it can be a pointer to 
//		anything.
//----------------------------------------------------------------------

void sl_append(struct SynchList *sl, void *item)
{
  lock_acquire(&sl->sl_lock);                // enforce mutual exclusive access to the list 
  struct SL_element *sl_elem = malloc(sizeof(struct SL_element));
  sl_elem->item = item;
  list_push_back(&sl->sl_list, &sl_elem->elem);
  cond_signal(&sl->sl_empty,&sl->sl_lock);  // wake up a waiter, if any
  lock_release(&sl->sl_lock);              
  return;
}


//----------------------------------------------------------------------
// SynchList::Remove
//      Remove an "item" from the beginning of the list.  Wait if
//	the list is empty.
// Returns:
//	The removed item. 
//----------------------------------------------------------------------

void *sl_remove(struct SynchList *sl)
{
  struct list_elem *e;
  void *item;
  lock_acquire(&sl->sl_lock);                // enforce mutual exclusion
  while(list_empty(&sl->sl_list)){
    cond_wait(&sl->sl_empty, &sl->sl_lock);  // wait until list isn't empty
  }
  e = list_pop_front(&sl->sl_list);
  struct SL_element *sl_elem = list_entry(e, struct SL_element, elem);
  item = sl_elem->item;
  free(sl_elem);
  lock_release(&sl->sl_lock);
  return item;
}

