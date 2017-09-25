#include <unistd.h>

int main(){
  sleep(1);
  write(1, "Hello world!\n", 13);
}
