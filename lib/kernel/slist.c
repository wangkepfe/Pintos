#include "slist.h"
#include "threads/malloc.h"
#include <stdio.h>
//        #include <stdlib.h>

/* List structure */
struct Node
{
  ListElement Element;
  Position    Next;
};

/* make empty list */

SList
MakeEmpty( SList L )
{
  if( L != NULL )
    DeleteList( L );
  L = malloc( sizeof( struct Node ) );
  if( L == NULL ) {
    printf( "Out of memory!\n" );
    return NULL;
  }
  L->Next = NULL;
  return L;
}

/* Return true if L is empty */

int
IsEmpty( SList L )
{
  return L->Next == NULL;
}

/* Return true if P is the last position in SList L */
/* Parameter L is unused in this implementation */

int IsLast( Position P, UNUSED SList L)
{
  return P->Next == NULL;
}

/* Return Position of X in L; NULL if not found */

Position
Find( ListElement X, SList L )
{
  Position P;

  P = L->Next;
  while( P != NULL && P->Element != X )
    P = P->Next;

  return P;
}

/* Delete from a SList */
/* Cell pointed to by P->Next is wiped out */
/* Assume that the position is legal */
/* Assume use of a header node */

void
Delete( ListElement X, SList L )
{
  Position P, TmpCell;

  P = FindPrevious( X, L );

  if( !IsLast( P, L ) )  /* Assumption of header use */
    {                      /* X is found; delete it */
      TmpCell = P->Next;
      P->Next = TmpCell->Next;  /* Bypass deleted cell */
      free( TmpCell );
    }
}

/* If X is not found, then Next field of returned value is NULL */
/* Assumes a header */

Position
FindPrevious( ListElement X, SList L )
{
  Position P;

  P = L;
  while( P->Next != NULL && P->Next->Element != X )
    P = P->Next;

  return P;
}

/* Insert (after legal position P) */
/* Header implementation assumed */
/* Parameter L is unused in this implementation */

void
Insert( ListElement X, UNUSED SList L, Position P )
{
  Position TmpCell;

  TmpCell = malloc( sizeof( struct Node ) );
  if( TmpCell == NULL ) {
    printf( "Out of space!!!\n" );
    return;
  }

  TmpCell->Element = X;
  TmpCell->Next = P->Next;
  P->Next = TmpCell;
}

/*  DeleteSList algorithm */

void
DeleteList( SList L )
{
  Position P, Tmp;

  P = L->Next;  /* Header assumed */
  L->Next = NULL;
  while( P != NULL )
    {
      Tmp = P->Next;
      free( P );
      P = Tmp;
    }
}

Position
Header( SList L )
{
  return L;
}

Position
First( SList L )
{
  return L->Next;
}

Position
Advance( Position P )
{
  return P->Next;
}

ListElement
Retrieve( Position P )
{
  return P->Element;
}
