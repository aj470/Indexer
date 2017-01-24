/*
  Group: 1/0
  Net ID: gg548
          aj470
*/


#ifndef INDEXER
#define INDEXER

#include <stdlib.h>

#include "tokenizer.h"
#include "sorted-list.h"

SortedListPtr List;

typedef struct fRec_
{
	char* fname;
	int freq;
}fRec;


typedef struct Token
{
	char* word;
	SortedListPtr fList;
}Token;



/******************Comparator Functions**************/

int strComp(void *s1, void *s2);

int tokeComp(void *p1, void *p2);

int recComp(void *p1, void *p2);

int nameComp(void *p1, void *p2);

/*****************Destroy Functions**************/

void fRecDestroy(void *fr);

void tokenDestroy(void *tk);


char* generatePath(char * file);

void processToken(char* word, char* filename);

void writeToOutput (char * file);

void addTokenToList(char* word, char* filename);

int traverseInputFile(FILE* file, char* inputname);

int traverseDirectories(char* inputname);

#endif
