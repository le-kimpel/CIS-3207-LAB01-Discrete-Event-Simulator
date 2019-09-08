#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *read_file();


int *read_file(){

    char keys[100][12] =
      {"SEED", "INIT_TIME", "FIN_TIME", "ARRIVE_MIN", "ARRIVE_MAX", "QUIT_PROB",
	"CPU_MIN", "CPU_MAX", "DISK1_MIN", "DISK1_MAX", "DISK2_MIN", "DISK2_MAX"};
    
    int *vals = (int*)malloc(sizeof(int)*12); //12 for number of entries in config file
    FILE *fp = fopen("config.txt", "r");
    if(fp == NULL){
      puts("file cannot be read or there is no file.");
    }

   
    char buffer[100];
    int count = 0;
while(fgets(buffer, sizeof(buffer), fp) != NULL){
     
      char search[100];
      strcpy(search, keys[count]);
      strcat(search, " %ld");

      if(sscanf(buffer, search, &vals[count]) != 0){
	count++;
      }

    }
 fclose(fp);
 
 return vals;
}




