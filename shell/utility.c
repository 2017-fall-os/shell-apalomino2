#include <unistd.h>
#include <stdlib.h>
#include "utility.h"

void print(char *buf){
  write(1, buf, strlen(buf));
}

void println(char *buf){
  print(buf);
  print("\n");
}

void printerr(char *buf){
  write(2, buf, strlen(buf));
}

int strlen(char *str){
  int count = 0;
  while(*str){
    count++;
    str++;
  }
  return count;
}

int strcmp(char *str1, char *str2){
  while(*str1 && *str2){
    if(*str1 != *str2)
      break;
    str1++;
    str2++;
  }
  return *str2 - *str1;
}

char *strcat(char *dest, char *src){
  int destlen = strlen(dest);
  int srclen = strlen(src);

  char *str = calloc(destlen+srclen+1, sizeof(char));
  char *temp = str;
  while(*dest){
    *temp = *dest;
    dest++;
    temp++;
  }
  while(*src){
    *temp = *src;
    src++;
    temp++;
  }
  return str;
}
