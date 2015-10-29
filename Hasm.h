#include <stdio.h>
#include <stdarg.h>
#include "Parser.h"
#include "SymbolTable.h"

void assemble(char* ifn, char* ofn); //assembles input filename into an output filename
int firstPass(SymbolTable* symtbl); //first pass to populate the symbol table
void secondPass(SymbolTable* symtbl, unsigned char* out); //writes code into out
void error(int sev, char* msg, ...); //error. takes message and severity

extern int errflag; //whether error flag is set
extern char* fn; //filename

