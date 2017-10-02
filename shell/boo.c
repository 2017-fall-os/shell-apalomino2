#include <unistd.h>

int main(int argc, char ** argv, char **envp){
  sleep(3);
  write(1, "boo!\n", 5);
}
