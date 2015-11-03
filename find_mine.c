// Fine A mine in c
#include <stdio.h>
#include <time.h>

void random(void){
  srand(time(NULL));
}


int main(void){
  int i;

  random();
  for (i=0; i<5 ; i++){
    printf("Random number: %d\n",rand());
  }
  return 0;
}







