// boundedbuffer.cc
//  This is a skeleton for the implementation of bounded buffer to test
//  synchronization primitives (condition and lock).
//
//  Your implementation of bounded buffer should be thread safe. For an
//  example of implementation refer to synchlist implementation.
//
// Created by Andrzej Bednarski
//
// Modified by Vlad Jahundovics (translation from C++ to C)

#include "threads/boundedbuffer.h"
#include "threads/malloc.h"

void bb_init(struct bounded_buffer *bb, int _size)
{
  bb->size = _size;
  // write your code here
}

void bb_destroy(struct bounded_buffer *bb)
{
  // write your code here
}

int bb_read(struct bounded_buffer *bb)
{
  // write your code here
}

void bb_write(struct bounded_buffer *bb, int value)
{
  // write your code here
}

