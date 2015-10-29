#include "Hasm.h"

int errflag;
char* fn;

void assemble(char* ifn, char* ofn)
{
  FILE *ifp; //input file
  FILE *ofp; //output file
  int isz; //input size
  int osz; //output size
  char* ibuf; //input buffer
  unsigned char* obuf; //output buffer
  errflag = 0; //error flag
  fn = ifn;

  SymbolTable symtbl; //symbol table

  if((ifp = fopen(ifn, "rb")) == NULL)
  {
    error(2, "Failed to open %s\n", ifn);
    return;
  }
  
  fseek(ifp, 0, SEEK_END);
  isz = ftell(ifp);
  rewind(ifp);

  ibuf = (char*) calloc(isz+1, sizeof(char));
  fread(ibuf, 1, isz, ifp);
  fclose(ifp);

  initTable(&symtbl);
  initParse(ibuf);
  osz = firstPass(&symtbl);

  if(errflag > 1)
  {
    fprintf(stderr, "Errors assembling: %s\n", ifn);
    clearHash(symtbl.table);
    free(symtbl.table);
    free(ibuf);
    return;
  }

  obuf = (unsigned char*) calloc((osz*17)+1, sizeof(char));
  secondPass(&symtbl, obuf);

  if(errflag > 1)
    fprintf(stderr, "Errors assembling: %s\n", ifn);
  else if((ofp = fopen(ofn, "wb")) == NULL)
    error(3, "Failed to open output file\n");
  else
  {
    fwrite(obuf, sizeof(char), osz*2, ofp);
    fclose(ofp);
  }

  free(obuf);
  free(ibuf);
  clearHash(symtbl.table);
  free(symtbl.table);
}

int firstPass(SymbolTable* symtbl)
{
  int res = 0;
  while(hasMoreCommands())
  {
    advance();
    char* tmp;
    switch(commandType())
    {
      case A_COMMAND:
        res++;
        break;
      case L_COMMAND:
        tmp = (char*) calloc(strlen(_parser_com) - 1, sizeof(char));
	parseSymbol(tmp);
	addEntry(symtbl, tmp, res);
	free(tmp);
        break;
      case C_COMMAND:
        res++;
        break;
    }
  }
  _parser_cur = 0;
  _parser_ln = 0;
  resetCmd();

  return res;
}

void secondPass(SymbolTable* symtbl, unsigned char* out)
{
	int outp = 0;
  //strcpy(out, "");
  while(hasMoreCommands())
  {
    advance();
    unsigned short op = 0; //number for op code
    unsigned char* bin = (unsigned char*) calloc(3, sizeof(char)); //binary string
    char* tmp;
    switch(commandType())
    {
      case A_COMMAND:
        tmp = (char*) calloc(strlen(_parser_com), sizeof(char));
				parseSymbol(tmp);
				if(getAddress(symtbl, tmp) == -1)
					error(2, "%s:%d: Error: Invalid Label `%s'/n", fn, _parser_ln, tmp);

        op = getAddress(symtbl, tmp);
				parseBin(bin, op);
				memcpy(out + outp, bin, 2); //copy data to output string
				outp += 2; //increase pointer by 2
				free(tmp);
        break;
      case C_COMMAND:
        tmp = (char*) calloc(strlen(_parser_com), sizeof(char));
				parseDest(tmp);
				op += destCode(tmp);

				parseJump(tmp);
				if(jumpCode(tmp) == -1)
					error(2, "%s:%d: Error: Invalid Jump command `%s'\n", fn, _parser_ln, tmp);

        op += jumpCode(tmp);

        parseComp(tmp);
				if(compCode(tmp) == -1)
	  			error(2, "%s:%d: Error: Invalid Code command `%s'\n", fn, _parser_ln, tmp);
	
				op += compCode(tmp);

				op += 57344; //magic number for c commands
				parseBin(bin, op);
				memcpy(out + outp, bin, 2); //copy data to output string
				outp += 2; //increase pointer by 2
				free(tmp);
        break;
    }
    free(bin);
  }
  resetCmd();
}

void error(int sev, char* msg, ...)
{
  va_list arg;
  va_start(arg, msg);

  vfprintf(stderr, msg, arg);
  va_end(arg);

  if(sev > errflag)
    errflag = sev;
}

