 /*
  Group: 1/0
  Net ID: gg548
          aj470
 */


#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>

#include "indexer.h"
#include "tokenizer.h"
#include "sorted-list.h"

char *currName = "";


/*****************Comparator Functions**************/

int strComp(void *s1, void *s2)
{
	return strcmp((char *)s1, (char *)s2);
}


int tokeComp(void *p1, void *p2)
{
	Token *t1 = (Token*)p1;
	Token *t2 = (Token*)p2;

	return strcmp((char *)t2->word, (char *)t1->word);
}


int recComp(void *p1, void *p2)
{
	fRec *rec1 = (fRec*)p1;
	fRec *rec2 = (fRec*)p2;

	if(rec1->freq == rec2->freq)
	{
		return strComp(rec2->fname,rec1->fname);
	}
	else
	{
		return rec1->freq - rec2->freq;
	}
}


int nameComp(void *p1, void *p2)
{
	fRec *t1 = (fRec*)p1;
	fRec *t2 = (fRec*)p2;

	return strComp(t2->fname,t1->fname);
}


/*****************Destroy Functions**************/

void fRecDestroy(void *fr)
{
	if (fr == NULL)
	{
		printf("ERR: File record does not exist.");
	}
	else 
	{
    	fRec *rec = fr;
    	free(rec);
  	}
}

void tokenDestroy(void *tk)
{
	Token *toke = tk;
	free(toke->word);
	free(toke->fList);
	free(toke);
}

/*
  Creates path to be stored in fRec struct
 */
char* generatePath(char * file)
{
	size_t fileLength = strlen(file)+1; //get length of file path string

	//if path name exceeds max, exit
	if(fileLength > PATH_MAX)
	{
		printf("ERR: Path for %s exceeds max length.\n",file);
		exit(-1);
	}

	//create vairable to hold path name
	char * path = (char*)malloc(sizeof(char) * fileLength);
	//check if malloc failed
	if((path == NULL))
	{
		printf("ERR: Memory allocation failed in 'generatePath'\n");
		return NULL;
	}
	//copy file into path and return 
	else
	{
		strcpy(path, file);
		return path;
	}
}



/*Function checks if desired output file exists then it will write JSON 
formatted inverted-index to desired output file*/
void writeToOutput (char * file)
{
	//check if there are tokens to be indexed
	if(SLisEmpty(List) == 1)
	{
	  printf("ERR: file cannot be indexed in 'writeToOutput'.\n");
	}
	//if sorted-list is not empty, check if file exists
	else
	{
		//if file exists, ask user if they wish to overwrite
		 if(access(file, F_OK) != -1)
        {
          printf("Output file requested already exists, overwrite? (y/n)\n");
          char input, check;
          scanf("%c", &input);
          check = tolower(input);

          if(check != 'y')
            {
	      printf("Indexing unsuccessful to output file. TRY AGAIN!\n");
              exit(0);
            }
        }
    }

  /*write to output file*/
	FILE *openFile = fopen(file, "w+");
  //open or create output file with write permission
	if (openFile != NULL)
	  {
      fprintf(openFile, "{\"list\" : [\n");
      SortedListIteratorPtr level1 = SLCreateIterator(List);
      Token *tokenword = SLGetItem(level1);
	  
      //loop to write tokens out it JSON format while SLGetItem returns a token
      while (tokenword != NULL) 
      {
        fprintf(openFile, "\t  {\"%s\" : [ \n", tokenword->word);
        SortedListIteratorPtr level2 = SLCreateIterator(tokenword->fList);
        fRec *filetoken = SLGetItem(level2);

	//loop to write out path of current and frequency of current token 
        while (filetoken != NULL) 
	{
          fprintf(openFile, "\t\t  {\"%s\" : %d}", filetoken->fname, filetoken->freq);
          if (!SLIsLastItem(level2))
            {
              fprintf(openFile, ",");
            }
          fprintf(openFile, "\n");
          filetoken = SLNextItem(level2);
        }

        fprintf(openFile, "\t  ]}");
	
        if (!SLIsLastItem(level1))
	  {
	    fprintf(openFile, ",");
	  }

        fprintf(openFile, "\n");
	
	//free allocated memory
        SLDestroyIterator(level2);

	//retrieve next token
        tokenword = SLNextItem(level1);
      }

      fprintf(openFile, "]}\n");
      //free allocated memory
      SLDestroyIterator(level1);
      //close file
      fclose(openFile);
      printf("Indexing successful to output file: %s\n", file);
    }

  else
    {
      printf("ERR in 'writeToOutput': File could not be opened");
    }
}

/*
Stores tokens into sorted-list, if new token it adds new object to sorted-list, 
otherwise, duplicates increase frequency of existing object	
*/
void addTokenToList(char* word, char* filename)
{
  Token *Find_token;
  fRec *Find_occurance;
  Token *allocate_token = malloc(sizeof(Token));
  fRec *allocate_flist = malloc(sizeof(fRec));
  
  //check if malloc failed
  if (allocate_flist == NULL) 
    {
      printf("ERR in 'addTokenToList': Malloc failed");
      exit(0);
    }
  else if(allocate_token == NULL)
    {
      printf("ERR in 'addTokenToList': Malloc failed");
      exit(0);
    }

  allocate_token->word = word;
  allocate_token->fList = NULL;
  allocate_flist->fname = generatePath(filename);
  allocate_flist->freq= 1;

  Find_token = SLSearch(List, allocate_token, nameComp);
  //check if token exists in list
  if(Find_token != NULL )
    {
      Find_occurance = SLSearch(Find_token->fList, allocate_flist, nameComp);
      //check if record exists, if so, add object to list and increment frequency
      if( Find_occurance != NULL )
	{
	  int freq = Find_occurance->freq;
	  allocate_flist->freq = freq+1;
	  SLInsert(Find_token->fList, allocate_flist);
	  SLRemove(Find_token->fList, Find_occurance);
	}
    else
      {
	SLInsert(Find_token->fList, allocate_flist);
      }
      //free allocated memory
    tokenDestroy(allocate_token);
  }
  //token does not exist, create new object and store to list
  else
    {
      allocate_token->fList = SLCreate(recComp, fRecDestroy);
      SLInsert(allocate_token->fList, allocate_flist);
      SLInsert(List, allocate_token);
    }
}

/*
Opens and closes file. Calls TKGetNextToken to retrieve a new token 
and sends it to addTokenToList
 */
int traverseInputFile(FILE* file, char* inputname)
{
  //allocate memory to retrieve tokens
  TokenizerT* tk = TKCreate(file);
  char* token; 
  token = TKGetNextToken(tk); // get first token
  if(token != NULL) //check if first token is null or not
    {
       //send token to be sorted into list
      addTokenToList(token, inputname);
    }
  //loop to retireve tokens from file until end is reached
  while((token = TKGetNextToken(tk)) != NULL)
    {       //send token to be sorted into list
	addTokenToList(token, inputname);
    }

  //free allocated memory
  TKDestroy(tk);
  return 0;
}

/*
Takes input file and checks if it is a directory, and has recursive calls for further
directories. If a file is reached, traverseFile is called.
*/

int traverseDirectories(char* inputname)
{
  
  void *dp = opendir(inputname);
 
 struct dirent *entry;
 /* Current recursive iteration is a directory */
 if(dp != NULL)
   {
     DIR *directory = (DIR *)dp;
     LOOP: while(1)
     {
	 entry = readdir(directory);
	 currName = entry->d_name;
	 ///check if directory is empty
	 if(entry == NULL)
	   {
	     break;
	   }

	 //make sure directory was not yet explored
	 if(!strcmp(entry->d_name,".") || !strcmp(entry->d_name,".."))
	   {
	        goto LOOP;
	   }

	 // Save filepath
	 char* next = (char *)malloc(strlen(inputname) + strlen(entry->d_name) + 2);
	 strcpy(next, inputname);
	 strcat(next, "/");
	 strcat(next, entry->d_name);

	 // Recursive call if another directory is found
	 traverseDirectories(next);
	 free(next);
       }
     closedir(directory);

		
   }

  if(dp == NULL)
    {
      // state = 'N';
     void *fp = fopen(inputname, "r+");

    if(fp != NULL)
       {
	 FILE *file = (FILE *)fp;
	 char * without = currName;
	 int i = 0;
	 int n = strlen(currName);

	 for(i = 0; i < n; i++)
	   {
	     if(without[i] != '.')
	       {
		 currName[i] =  without[i];
}
	     else
	       {
		 currName[i] = '\0';
		 break;
	       }
	   }


      traverseInputFile(file, currName);
      fclose(file);
    }
    else
      {
        printf("ERR: Cannot open file");
        return 0;
      }
    }
  return 0;
}
