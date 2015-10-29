#include "SymbolTable.h"

void initTable(SymbolTable* tbl)
{
  tbl->count = 16;
  tbl->table = (bucket**) calloc(HASH_SIZE, sizeof(bucket*));
  
  //stack pointers
  addEntry(tbl, "SP",     0x0000);
  addEntry(tbl, "LCL",    0x0001);
  addEntry(tbl, "ARG",    0x0002);
  addEntry(tbl, "THIS",   0x0003);
  addEntry(tbl, "THAT",   0x0004);
  addEntry(tbl, "SCREEN", 0x4000);
  addEntry(tbl, "KBD",    0x6000);

  //registers
  addEntry(tbl, "R0",  0x0000);
  addEntry(tbl, "R1",  0x0001);
  addEntry(tbl, "R2",  0x0002);
  addEntry(tbl, "R3",  0x0003);
  addEntry(tbl, "R4",  0x0004);
  addEntry(tbl, "R5",  0x0005);
  addEntry(tbl, "R6",  0x0006);
  addEntry(tbl, "R7",  0x0007);
  addEntry(tbl, "R8",  0x0008);
  addEntry(tbl, "R9",  0x0009);
  addEntry(tbl, "R10", 0x000a);
  addEntry(tbl, "R11", 0x000b);
  addEntry(tbl, "R12", 0x000c);
  addEntry(tbl, "R13", 0x000d);
  addEntry(tbl, "R14", 0x000e);
  addEntry(tbl, "R15", 0x000f);
}

void addEntry(SymbolTable* tbl, char* sym, int addr)
{
  bucket* tmp = contains(tbl->table, sym);
  bucket* new;

  if(tmp == NULL) //checks if new entry exists
  {
    tmp = tbl->table[hash(sym)]; //creates entry
    new = (bucket*) malloc(sizeof(bucket));
    new->sym = strdup(sym);
    new->next = NULL;
    if(addr == -1)
      new->address = tbl->count++;
    else
      new->address = addr;

    if(tmp == NULL) //for head
    {
      tbl->table[hash(sym)] = new; //I have no idea why this won't use tmp
      return;
    }

    while(tmp->next != NULL) //for linked list
    {
      tmp = tmp->next;
    }

    tmp->next = new;
  }
}

bucket* contains(bucket** tbl, char* sym)
{
  bucket* tmp = tbl[hash(sym)]; //nice short function courtesy of vible

  while(tmp != NULL)
  {
    if(strcmp(sym, tmp->sym) == 0)
      break;

    tmp = tmp->next;
  }

  return tmp;
}

int getAddress(SymbolTable* tbl, char* sym)
{
  bucket *tmp;
  int res = -2;
  
  if(sym[0] > 47 && sym[0] < 58)
    res = toNum(sym);

  if(res != -2)
    return res; //label begins with number

  if((tmp = contains(tbl->table, sym)) == NULL)
  {
    res = tbl->count;
    addEntry(tbl, sym, -1);
    return res;
  }
  return tmp->address;
}

void clearHash(bucket** tbl)
{
  for(int i = 0; i<HASH_SIZE; i++)
  {
    if(tbl[i] == NULL)
      continue;

    killNode(tbl[i]);
  }
}

void killNode(bucket* n)
{
  if(n->next != NULL)
    killNode(n->next);

  free(n->sym);
  free(n);
  return;
}

void iterateHash(bucket** tbl)
{
  bucket* tmp;
  for(int i = 0; i<HASH_SIZE; i++)
  {
    tmp = tbl[i];
    while(tmp != NULL)
    {
      printf("----------------\n");
      printf("Key: %s, Val: %d\n", tmp->sym, tmp->address);
      printf("Next: %p\n", (void*)tmp->next);

      tmp = tmp->next;
    }
  }
  printf("===================\n");
}

int toNum(char* s)
{
  int res = 0;
  int mul = 1; //multiplier
  for(unsigned int i = strlen(s)-1; i < strlen(s); i--) //fancy underflow magic
  {
    if(s[i] < 48 || s[i] > 57)
      return -1;

    res += (s[i] - 48) * mul;
    mul *= 10;
  }
  return res;
}

void debugString(char* s)
{
  printf("DEBUG:");
  for(unsigned int i=0; i<strlen(s); i++)
  {
    printf(" %d", s[i]);
  }
  printf("\n");
}

int hash(char* key)
{
  int res = 0;
  for(unsigned int i = 0; i < strlen(key); i++)
  {
    res += key[i];
  }

  return res % HASH_SIZE;
}

char* strdup(char* str)
{
  int n = strlen(str) + 1;
  char* dup = malloc(n);
  if(dup)
    strcpy(dup, str);

  return dup;
}

