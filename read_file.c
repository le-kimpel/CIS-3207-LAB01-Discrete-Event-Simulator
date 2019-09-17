#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float *read_file();

//reads a file as a set of keys
//then returns a pointer to float upon success 
float *read_file(){

  //keys to search for in document
    char keys[100][12] =
      {"SEED", "INIT_TIME", "FIN_TIME", "ARRIVE_MIN", "ARRIVE_MAX", "QUIT_PROB",
	"CPU_MIN", "CPU_MAX", "DISK1_MIN", "DISK1_MAX", "DISK2_MIN", "DISK2_MAX"};
    
    float *vals = (float*)malloc(sizeof(float)*12); //12 for number of entries in config file
    FILE *fp = fopen("config.txt", "r");
    //if unsuccessful
    if(fp == NULL){
      puts("file cannot be read or there is no file.");
    }

    //memory allocation and while loop initializers
    char buffer[100];
    int count = 0;

    //searches in the keys array for floats
    //if there are inadequate values, discard the search
    //return the correct vals as a pointer to a float
    while(fgets(buffer, sizeof(buffer), fp) != NULL){
     
      char search[100];
      strcpy(search, keys[count]);
      strcat(search, " %f");
      
      if(sscanf(buffer, search, &vals[count]) != 0){
	
	if (vals[count] < -.01){
	  puts("ERROR: cannot have negative values in config");
	  exit(0);
	}

	count++;

      }
      

    }
 fclose(fp);
 
 return vals;
}




