/*
  Group: 1/0
  Net ID: gg548
          aj470
*/


#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdlib.h>

typedef int (*CompareFuncT)( void *, void * );
typedef void (*DestructFuncT)( void * );

typedef struct Node
{
  void *data;
  int ptrCount;
  struct Node *next;
  struct Node *prev;
}Node;

struct SortedList
{
  Node *head; //pointer to head of linked list
  int total;      //holds number of nodes in list
  CompareFuncT compFunc; //pointer to compare funtion
  DestructFuncT destFunc;  //pointer to destructor funtion
};
typedef struct SortedList* SortedListPtr;

struct SortedListIterator
{
  SortedListPtr listPtr;
  Node *search;
  DestructFuncT destFunc;

};
typedef struct SortedListIterator* SortedListIteratorPtr;

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df);

void SLDestroy(SortedListPtr list);

int SLInsert(SortedListPtr list, void *newObj);

int SLRemove(SortedListPtr list, void *newObj);

SortedListIteratorPtr SLCreateIterator(SortedListPtr list);

void SLDestroyIterator(SortedListIteratorPtr iter);

void * SLNextItem(SortedListIteratorPtr iter);

void * SLGetItem( SortedListIteratorPtr iter );

void * SLSearch(SortedListPtr list, void *Obj, CompareFuncT cf);

int SLIsLastItem(SortedListIteratorPtr iter);

int SLisEmpty(SortedListPtr list);

#endif


