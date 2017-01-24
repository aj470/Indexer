/*
  Group: 1/0
  Net ID: gg548
          aj470
*/


#ifndef TOKENIZER
#define TOKENIZER

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef struct TokenizerT_{
	FILE *file;
} TokenizerT;

/*
	Initializes tokenizer structure for parsing. 
*/
TokenizerT *TKCreate(FILE *file);

/*
	Parses through an input file and populates token buffers with alphanumeric characters until 
	a non-alphanumeic character or EOF is encountered.  This token is returned by TKGetNextToken().
*/
char *TKGetNextToken(TokenizerT *tk);


/*
	Destroys tokenizer structure.
*/
void TKDestroy(TokenizerT *tk);

#endif
