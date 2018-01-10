/* SList -- simple list for students. 
 * This list was not originally in the pintos implementation. 
 */
typedef void * ListElement;

#ifndef _SList_H
#define _SList_H

struct Node;
typedef struct Node *PtrToNode;
typedef PtrToNode SList;
typedef PtrToNode Position;

SList MakeEmpty( SList L );
int IsEmpty( SList L );
int IsLast( Position P,  SList L);
Position Find( ListElement X, SList L );
void Delete( ListElement X, SList L );
Position FindPrevious( ListElement X, SList L );
void Insert( ListElement X, SList L, Position P );
void DeleteList( SList L );
Position Header( SList L );
Position First( SList L );
Position Advance( Position P );
ListElement Retrieve( Position P );

#endif    /* _SList_H */
