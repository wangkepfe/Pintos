#include<stdio.h>
#include<stdlib.h>

struct list_item{
  int data;
  struct list_item* next;
};

void append(struct list_item *first, int x); /* puts x at the end of the list */
void prepend(struct list_item *first, int x); /* puts x at the beginning of the list */
void print(struct list_item *first);  /* prints all elements in the list */
/* input_sorted: find the first element in the list larger than x
   and input x right before that element */
void input_sorted(struct list_item *first, int x);
void clear(struct list_item *first); /* free everything dynamically allocated */

void append(struct list_item *first, int x){
  struct list_item *curr = first;
  while(curr->next != NULL)
    curr = curr->next;
  curr->next = (struct list_item*)malloc(sizeof(struct list_item));
  curr->next->data = x;
  curr->next->next = NULL;
}

void prepend(struct list_item *first, int x){
  struct list_item *newnode = (struct list_item*)malloc(sizeof(struct list_item));
  newnode->data = x;
  newnode->next = first->next;
  first->next = newnode;
}

void print(struct list_item *first){
  struct list_item *curr = first;
  printf("begin->");
  while(curr->next != NULL){
    printf("%d->",curr->next->data);
    curr = curr->next;
  }
  printf("end\n");
}

void input_sorted(struct list_item *first, int x){
  struct list_item *curr = first;
  while(curr->next != NULL && x > curr->next->data)
    curr = curr->next;
  struct list_item *newnode = (struct list_item*)malloc(sizeof(struct list_item));
  newnode->data = x;
  newnode->next = curr->next;
  curr->next = newnode;
}

void clear(struct list_item *first){
  struct list_item *curr = first->next, *deletenode;
  while(curr != NULL){
    deletenode = curr;
    curr = curr->next;
    free(deletenode);
  }
  first->next = NULL;
}

int main(){
  struct list_item *mylist = (struct list_item*)malloc(sizeof(struct list_item));
  mylist->data = -1;
  mylist->next = NULL;

  print(mylist);

  append(mylist,1);
  append(mylist,2);
  append(mylist,3);
  append(mylist,4);
  append(mylist,5);

  print(mylist);
  clear(mylist);

  prepend(mylist,1);
  prepend(mylist,2);
  prepend(mylist,3);
  prepend(mylist,4);
  prepend(mylist,5);

  print(mylist);
  clear(mylist);

  input_sorted(mylist,2);
  input_sorted(mylist,1);
  input_sorted(mylist,4);
  input_sorted(mylist,5);
  input_sorted(mylist,3);

  print(mylist);
  clear(mylist);
  print(mylist);

  free(mylist);
}
