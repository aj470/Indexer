/*
  Group: 1/0
  Net ID: gg548
          aj470
*/


#include <stdlib.h>
#include <stdio.h>
#include "sorted-list.h"

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df)
{
  SortedListPtr slp = (SortedListPtr) malloc(sizeof( struct SortedList)); //allocate memory for list pointer
	
	if (slp == NULL) 
	{
	        printf("Malloc failed\n");
		return NULL;
	}
	
	//intitialize list struct elements
	slp->head = malloc(sizeof(Node));	//set head pointer to NULL
	slp->head->data = NULL;	//set data stored in list head to NULL
	slp->head->next = slp->head;	//set next node pointer to head
	slp->head->prev = slp->head; //set prev node pointer to head
	slp->compFunc = cf;  //store comparator function into list pointer
	slp->destFunc = df;  //store destructor function into list pointer
	
	return slp; //return list pointer
}


void SLDestroy(SortedListPtr list)
{
	if(list == NULL)
	{
		return;
	}
   Node *current = list->head->next;
    
   while (current != list->head) 
   {
       current->prev->next = current->next;
       current->next->prev = current->prev;
       list->destFunc(current->data);
       free(current);
       current = list->head->next;
   }
  
   free(list->head);
   free(list);
}




int SLInsert(SortedListPtr list, void *newObj)
{
	typedef enum {FALSE, TRUE} bool;
    bool added = FALSE; //flag to check if item to be inserted is uniqueness
	Node *current; //pointer to traverse list when inserting new items
	current = list->head->next; //set pointet to start of list 
	
	while(!added && current != list->head)
	{
		if(list->compFunc(newObj, current->data) > 0)
		{
			break;
		}
		current = current->next;
	}
	
	Node *add = malloc(sizeof(Node));
	if(add == NULL)
	{
		printf("ERR: Malloc failed");
		added = FALSE;
	}
	else
	{
		add->data = newObj;
		add->prev = current->prev;
		current->prev->next = add;
		add->next = current;
		current->prev = add;
		add->ptrCount = 0;
		added = TRUE;
	}		
		
	if(added == TRUE)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}



int SLRemove(SortedListPtr list, void *newObj)
{
	//local variable declarations
	Node *current = list->head->next;	//begin with the head of the list
	typedef enum {FALSE, TRUE} bool;
	bool found = FALSE;				//flag for record match

	while(current != list->head)
	{
		if(list->compFunc(newObj, current->data) == 0)
		{
			break;
		}
		current = current->next;
	}
	
	if(current == list->head)
	{
	  printf(" ");
	  //found == FALSE;
	}
	else
	{
		current->next->prev = current->prev;
		current->prev->next = current->next;
		if(current->ptrCount == 0)
		{
			list->destFunc(current->data);
			free(current);
		}
		current->next = NULL;
		current->prev = NULL;
		found = TRUE;
	}
	
			
	if(found == FALSE)
	{
		printf("ERR: Object not found");
		return 0;
	}
	else
	{
		return 1;
	}
}



 /*
  * SLCreateIterator creates a SortedListIterator for the SortedList pointed to by 'list'.
  *
  * SLCreateIterator should return a non-NULL SortedListIteratorPtr on success.
  * SLCreateIterator should return a NULL SortedListIteratorPtr if it could not
  *  construct a SortedListIterator, or if the SortedListPtr parameter 'list' is NULL.
  */

 SortedListIteratorPtr SLCreateIterator(SortedListPtr list)
 {

   SortedListIteratorPtr creat = malloc(sizeof(struct SortedListIterator));
   
   if(list == NULL)
    {
       printf("Err: Empty list");    
       return NULL;
    }
   
   creat->listPtr = list;
   creat->search = list->head->next;
   creat->destFunc = list->destFunc;
   creat->search->ptrCount++;
   return creat;
 };

 /*
  * SLDestroyIterator destroys a SortedListIterator pointed to by parameter 'iter'.
  *
  * SLDestroyIterator should destroy the SortedListIterator, but should NOT
  *  affect the list used to create the SortedListIterator in any way.
  */

void SLDestroyIterator(SortedListIteratorPtr iter)
{
	if(iter == NULL)
	{
		printf("ERR: Iterator does not exist");
	}
	if(iter->search->ptrCount > 0)
	{
		iter->search->ptrCount--;
	}
	if(iter->search->ptrCount == 0 && iter->search->next == NULL)
	{
		iter->listPtr->destFunc(iter->search->data);
		free(iter->search);
	}
	free(iter);
}



 //===1.2: SortedList Iterator Get/Next Operations

 /*
  * SLNextItem returns a pointer to the data associated with the
  *  next item in the SortedListIterator's list
  *
  * SLNextItem should return a NULL if all of iter's elements have
  *  been iterated through.
  *
  * NB: Be sure to check the length of the list that SortedListIterator holds
  *         before attempting to access and return an item from it.
  *         If an item is removed from the list, making it shorter, be careful not
  *         to try to read and return an item off the end of the list.
  */
 //DONE
 void *SLNextItem(SortedListIteratorPtr iter)
 {
   if(iter == NULL)
     {
       printf("Err: List Empty");
       return NULL;
     }
   if(iter->search->next == NULL)
    {
		return NULL; 
    }
   if(iter->search->next != iter->listPtr->head)
	{
		iter->search->ptrCount--;
		iter->search = iter->search->next;
		iter->search->ptrCount++;
	}
	else
	  {
	    return NULL;
	  }
   return iter->search->data;
}

 /*
  * SLGetItem should return a pointer to the current data item
  *   The current data item is the item that was most recently returned by SLNextItem
  *   SLGetItem should not alter the data item that SortedListIterator currently refers to
  *
  * SLGetItem should return a NULL pointer if the SortedListIterator has been advanced
  *  all the way through its list.
  *
  * NB: Be sure to check the length of the list that SortedListIterator holds
  *         before attempting to access and return an item from it.
  *         If an items are removed from the list, making it shorter, be careful not
  *         to try to read and return an item off the end of the list.
  */
 //DONE
void *SLGetItem( SortedListIteratorPtr iter )
{
  if(iter == NULL)
  {
	  printf("ERR: Empty List");
	  return NULL;
  }
  else
		return iter->search->data;
}



void * SLSearch(SortedListPtr list, void *Obj, CompareFuncT cf)
{
	Node *current = list->head->next;

	while(current != list->head)
	{
		if(cf(Obj, current->data) == 0)
		{
			break;
		}
		current = current->next;
	}
	return current->data;
}



int SLIsLastItem(SortedListIteratorPtr iter)
{
  if (iter->search->next == iter->listPtr->head)
    return 1;
  else
    return 0;
}



int SLisEmpty(SortedListPtr list)
{
	if(list->head->next == list->head)
	{
		return 1;
	}
	return 0;
}





