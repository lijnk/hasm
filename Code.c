#include "Code.h"

short int destCode(char* m)
{
  short int res = 0;
  if(strchr(m, 'A'))
    res += 4;

  if(strchr(m, 'D'))
    res += 2;

  if(strchr(m, 'M'))
    res += 1;

  return res << 3;
}

short int compCode(char* m)
{
  if(strlen(m) < 1 || strlen(m) > 3)
    return -1;

  char* p = strchr(m, 'A');
  int res = 0; //result

  if(!((p != NULL) && ((p = strchr(m, 'M')) != NULL)))
  {
    if((p = strchr(m, 'M')) != NULL) //should still be grabbing from 'M'
      res |= 64;
  }
  else
    return -1;

  switch(m[0])
  {
    case '1':
      res += 21;
    case '0':
      if(strlen(m) > 1)
        return -1;
      res += 42;
      return res << 6;
    case '-':
      res++;
    case '!':
      if(strlen(m) < 2)
        return -1;
      res++;
      break;
    case 'D':
      res |= 12;
      break;
    case 'M':
    case 'A':
      res |= 48;
      break;
    default:
      return -1;
  }

  if(strlen(m) < 2)
    return res << 6;

  switch(m[1])
  {
    case 'D':
    case 'M':
    case 'A':
      if((res & 1) == 1)
      {
        if(strlen(m) > 2)
	  return -1;

        if(m[1] == 'D')
	  res |= 12;
	else
	  res |= 48;
	break;
      }
    case '1':
      if(strlen(m) > 2)
        return -1;

      if((res & 2) != 0)
      {
        switch(m[1])
	{
	  case 'D':
	    res |= 12;
	    break;
	  case 'M':
	  case 'A':
	    res |= 48;
	    break;
	  case '1':
	    res |= 56;
	    break;
	  default:
	    return -1;
	}
	break;
      }
      return -1;

    case '-':
    case '+':
      res |= 2;
      break;
    case '|':
    case '&':
      break;
    default:
      return -1;
  }

  if(strlen(m) < 3)
    return res << 6;

  if(((res & 12) != 12) && ((res & 48) != 48))
    return -1;

  switch(m[2])
  {
    case '1':
      switch(m[1])
      {
        case '+':
	  if((res & 12) == 12)
	    res |= 17;
	  else if((res & 48) == 48)
	    res |= 5;
	  return res << 6;
	case '-':
	  return res << 6;
	default:
	  return -1;
      }
      break;
    case 'D':
      if((res & 48) != 48)
        return -1;
      switch(m[1])
      {
        case '+':
	  res &= 66;
	  return res << 6;
	case '-':
	  res |= 5;
	  res &= 71;
	  return res << 6;
	case '|':
	  res &= 64;
	  res |= 21;
	  return res << 6;
	case '&':
	  res &= 64;
	  return res << 6;
	default:
	  return -1;
      }
    case 'M':
    case 'A':
      if((res & 12) != 12)
        return -1;
      switch(m[1])
      {
        case '+':
	  res &= 66;
	  return res << 6;
	case '-':
	  res |= 17;
	  res &= 83;
	  return res << 6;
	case '|':
	  res &= 64;
	  res |= 21;
	  return res << 6;
	case '&':
	  res &= 64;
	  return res << 6;
	default:
	  return -1;
      }
    default:
      return -1;
  }
}

/*
short int compCode(char* m)
{

   * Notable Patterns
   * Single D rule 0 0 1 1 X X - +12
   * Single A rule 1 1 0 0 X X - +48
   * D and A rule  0 0 0 0 X X - +0
   * constant rule 1 X 1 X 1 X - +42
   * X+1 rule      X 1 X 1 X 1 - +21
   * +/- rule      X X X X 1 X - +2
   * ! rule        X X X X X 1 - +1
   * & rule        X 0 X 0 0 0 - +0
   * | rule        X 1 X 1 0 1 - +21
   *
   * 0 is          X 0 X 0 X 0 - +0
   * 1 is          X 1 X 1 X 1 - +21
   * -1 is         X 1 X 0 X 0 - +16
   *
   * D-A is        X 1 X 0 X 1 - +17
   * A-D is        X 0 X 1 X 1 - +5
   *
   * if D then check for A
   * 
   *

  struct {
    char* op;
    short int v;
  } cop[] = {
    {"0"  , 42},
    {"1"  , 63},
    {"-1" , 58},
    {"D"  , 12},
    {"A"  , 48},
    {"M"  ,112},
    {"!D" , 13},
    {"!A" , 49},
    {"!M" ,113},
    {"-D" , 15},
    {"-A" , 51},
    {"-M" ,115},
    {"D+1", 31},
    {"A+1", 55},
    {"M+1",119},
    {"D-1", 14},
    {"A-1", 50},
    {"M-1",114},
    {"D+A",  2},
    {"D+M", 66},
    {"D-A", 19},
    {"D-M", 83},
    {"A-D",  7},
    {"M-D", 71},
    {"D&A",  0},
    {"D&M", 64},
    {"D|A", 21},
    {"D|M", 85},
  };

  for(int i = 0; i<28; i++)
    if(strcmp(m, cop[i].op) == 0)
      return cop[i].v << 6;
  
  return -1;
}
*/

short int jumpCode(char* m)
{
  char* jop[] = {
    "",
    "JGT",
    "JEQ",
    "JGE",
    "JLT",
    "JNE",
    "JLE",
    "JMP"
  };

  for(short int i=0; i<8; i++)
    if(strcmp(m, jop[i]) == 0)
      return i;
  
  return -1;
}

