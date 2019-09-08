#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//random generator
int random_gen(int max, int min);
_Bool prob_select(float percentage);

int random_gen(int max, int min){

  int range = max-min;
  srand(time(0)); //initializes generator
  
  return rand()%range;
}

  
//probability selector
_Bool prob_select(float percentage){
  if (percentage < 0 || percentage > 100){
    return 0;
  }else{
    int temp = random_gen(0,100);
    if (temp <= percentage){
      return 1;
    }else{
      return 0;
    }
  }
}
