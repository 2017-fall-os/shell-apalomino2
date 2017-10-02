#include <unistd.h>
#include "utility.h"

int main(int argc, char **argv, char ** envp){
  for(int i=0; argv[i]; i++){
    write(1, argv[i], strlen(argv[i]));
    write(1, "\n", 1);
  }
}
