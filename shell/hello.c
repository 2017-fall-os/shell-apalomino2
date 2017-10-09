#include <unistd.h>

int main(){
  for(int i=0; i<10; i++){
    sleep(1);
    write(1, "\nHello world!\n", 14);
  }
}
