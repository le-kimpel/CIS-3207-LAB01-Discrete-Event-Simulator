#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "read_file.h"

//random generator
int random_gen(int max, int min);
_Bool prob_select(float percentage);

//uses the random generator equation from lab documentation
int random_gen(int max, int min){  
  return rand()%(max-min+1)+min;
}

  
//probability selector
//finds a random between max, min.
//RETURN 1 means job LEAVES cpu
//RETURN 0 means job REMAINS in the simulation
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
