#include <stdio.h>
#include "Hasm.h"

int main(int argc, char** argv)
{
  if(argc < 2)
    return 1;
 
  for(int i=1; i<argc; i++)
  {
    char* tp = strrchr(argv[i], '.');
    char* ofn = calloc((tp-argv[i]) + 6, sizeof(char));
    strncpy(ofn, argv[i], tp-argv[i]);
    strcat(ofn, ".rom");

    assemble(argv[i], ofn);
    free(ofn);
  }

  return 0;
}

/* TODO: cli arguments
 * TODO: separate SymbolTable and HashTable (so HashTable can be used separately in the future)
 */

