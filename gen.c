#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "read_file.h"

//random generator
int random_gen(int max, int min);
_Bool prob_select(float percentage);

int random_gen(int max, int min){

  
  return rand()%(max-min+1)+min;
}

  
//probability selector
_Bool prob_select(float percentage){
  if (percentage < 0 || percentage > 100){
    return 0;
  }else{
    int temp = random_gen(100,0);
    if (temp <= percentage){
      return 1;
    }else{
      return 0;
    }
  }
}
