#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define HASH_SIZE 31

struct node
{
  char* sym; //symbol
  int address; //address
  struct node* next;
};

typedef struct node bucket;

struct head
{
  bucket** table;
  int count;
};

typedef struct head SymbolTable;

void initTable(SymbolTable* tbl); //initialises table
void addEntry(SymbolTable* tbl, char* sym, int addr); //adds entry to table
bucket* contains(bucket** tbl, char* sym); //checks if table contains entry
int getAddress(SymbolTable* tbl, char* sym); //gets address of entry. if it doesn't exist, it adds one

void clearHash(bucket** tbl); //clears entire hashtable
void killNode(bucket* n); //recursively kills nodes in linked list
void iterateHash(bucket** tbl); //iterates through hashtable and prints entries
int toNum(char* s); //simple tonum function
void debugString(char* s); //prints string out in numbers
int hash(char* key); //hash function

char* strdup(char* str); //strdup because it's not standard

