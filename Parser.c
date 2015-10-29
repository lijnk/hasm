#include "Parser.h"


char* _parser_stream;
int _parser_cur;
char* _parser_com;
int _parser_sz;
int _parser_ln;
int _parser_os;

//SymbolTable symtbl;

void initParse(char* stream)
{
  _parser_stream = stream;
  _parser_cur = 0;
  _parser_com = NULL;
  _parser_sz = strlen(stream);
  _parser_ln = 0;
}

bool hasMoreCommands()
{
  if(_parser_cur < _parser_sz)
    return true;
  return false;
}

void resetCmd()
{
  if(_parser_com != NULL)
  {
    //printf("CLEARING %d : %p : %s\n", _parser_ln, _parser_com, _parser_com);
    free(_parser_com);
    _parser_com = NULL;
  }
}

void advance()
{
  resetCmd();

  char* sp = _parser_stream+_parser_cur; //start/end pointers
  char* ep = strchr(sp, '\n');
  char* res;
  size_t size;

  if(ep == NULL) //if we don't have \n after final command
    ep = strchr(sp, 0);

  _parser_cur = (ep-_parser_stream) + 1;

  char* tp = strstr(sp, "//");
  if((tp != NULL) && (tp < ep)) //comments
    ep = tp;

  size = ep-sp; //copy string
  res = (char*) malloc(sizeof(char)*size);
  strncpy(res, sp, size);

  //_parser_com = (char*) malloc(sizeof(char)*size);
  int ws = 0; //whitespace offset
  unsigned int i = 0;

  for(i = 0; i < size; i++) //pass for the whitespace
  {
    if(res[i] == ' ' || res[i] == '\t' || res[i] == '\r')
    {
      ws++;
      res[i] = ' ';
    }
  }

  _parser_com = (char*) malloc(sizeof(char)*(size-ws+1));
  ws = 0;
  for(i = 0; i<size; i++) //pass to copy into command
  {
    if(res[i] == ' ')
      ws++;
    else
      _parser_com[i-ws] = res[i];
  }
  _parser_com[size-ws] = 0;
  _parser_ln++;
  free(res);

  if(_parser_com[0] == 0 && hasMoreCommands())
  {
    resetCmd();
    advance();
    return;
  }
}

int commandType()
{
  if(_parser_com[0] == '@')
    return A_COMMAND;

  if(_parser_com[0] == '(' && _parser_com[strlen(_parser_com)-1] == ')')
    return L_COMMAND;

  if(strchr(_parser_com, '=') != NULL || strchr(_parser_com, ';') != NULL)
    return C_COMMAND;

  return U_COMMAND;
}

void parseSymbol(char* d)
{
  if(commandType() == A_COMMAND)
  {
    strncpy(d, _parser_com+1, strlen(_parser_com)-1);
    d[strlen(_parser_com)-1] = 0;
  }
  else if(commandType() == L_COMMAND)
  {
    strncpy(d, _parser_com+1, strlen(_parser_com)-2);
    d[strlen(_parser_com)-2] = 0;
  }

}

void parseDest(char* d)
{
  char* tp = strchr(_parser_com, '=');
  if(tp != NULL)
  {
    strncpy(d, _parser_com, tp-_parser_com);
    d[tp-_parser_com] = 0;
  }
}

void parseComp(char* d)
{
  char* tp1 = strchr(_parser_com, '=');
  char* tp2 = strchr(_parser_com, ';');
  
  if(tp1 == NULL)
    tp1 = _parser_com;
  else
    tp1++;

  if(tp2 == NULL)
    tp2 = _parser_com + strlen(_parser_com);

  strncpy(d, tp1, tp2-tp1);
  d[tp2-tp1] = 0;
}

void parseJump(char* d)
{
  char* tp = strchr(_parser_com, ';');

  if(tp != NULL)
    strcpy(d, tp+1);
  else
    strcpy(d, "");

}

void parseBin(unsigned char* o, unsigned short int v)
{
	o[0] = (char) (v & 0xFF);
	o[1] = (char) ((v >> 8) & 0xFF);
	//printf("%.4X : %.2X %.2X\n", v, o[1], o[0]);
/*
  char res[18];
  for(int i=0; i<16; i++)
  {
    if(((1<<i) & v) == (1<<i))
      res[15-i] = '1';
    else
      res[15-i] = '0';
  }
  res[16] = '\n';
  res[17] = '\0';

  strcpy(o, res);
*/
}

