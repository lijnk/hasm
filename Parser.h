#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Code.h"

enum {A_COMMAND=0, L_COMMAND, C_COMMAND, U_COMMAND};

//void parse(char* s, int sz, char* out); //parses file
void initParse(); //initialises parser
void resetCmd(); //resets command to null
bool hasMoreCommands(); //returns true if more commands are available
void advance(); //advances to next command
int commandType(); //returns command type
void parseSymbol(char* d); //returns symbol (A or L)
void parseDest(char* d); //returns dest (C)
void parseComp(char* d); //returns comp (C)
void parseJump(char* d); //returns jump (C)

void parseBin(unsigned char* o, unsigned short int v); //translates v into binary line

extern char* _parser_stream; //stream
extern int _parser_cur; //cursor
extern char* _parser_com; //current command
extern int _parser_sz; //size
extern int _parser_ln; //line number
extern int _parser_os; //out size

//extern SymbolTable symtbl; //symbol table (head of hashtable)

