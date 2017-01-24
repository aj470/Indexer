/*
  Group: 1/0
  Net ID: gg548
          aj470
*/

#include "sorted-list.h"
#include "tokenizer.h"
#include "indexer.h"


int main(int argc, char** argv)
{

  if(argc != 3)
    {
      printf("Invaild number of arguments: Program Aborted \n");
      exit(0);
    }
  else
    {
      List = SLCreate(tokeComp, tokenDestroy);

      traverseDirectories(argv[2]);

      writeToOutput(argv[1]);
      
      SLDestroy(List);
    }
  return 0;
}


