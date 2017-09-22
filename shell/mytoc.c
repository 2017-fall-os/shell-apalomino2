#include <unistd.h>
#include <stdlib.h>
#include "mytoc.h"
#include "utility.h"

int countTokens(char *buf, char delim){
  int count = 0;
  
  while(*buf){
    //buf is not 
    while(*buf == delim)
      buf++;
    if(*buf)
      count++;
    while(*buf && *buf != delim)
      buf++;
  }
  return count;
}

void fillVec(char **tokenVec, char *buf, char delim, int numTokens){
  char *temp;
  int numChars;
  while(numTokens){
    //get to the beginning of the token
    while(*buf == delim)
      buf++;

    //get token size
    temp = buf;
    numChars = 0;
    while(*temp && *temp != delim){
      numChars++;
      temp++;
    }

    //create and place token
    *tokenVec = calloc(numChars+1, sizeof(char));
    temp = *tokenVec;
    //copy each char
    while(*buf && *buf != delim){
      *temp = *buf;
      buf++;
      temp++;
    }

    tokenVec++;
    numTokens--;
  }
}

char ** mytoc(char *buf, char delim){
  //find out how many tokens first
  int numTokens = countTokens(buf, delim);

  //create token vector
  char **tokenVec;
  tokenVec = (char **) calloc(numTokens+1, sizeof(char *));
  fillVec(tokenVec, buf, delim, numTokens);
  
  return tokenVec;
}
