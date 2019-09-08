#include <stdio.h>
//#include "components.h"
#include "gen.h"
#include "read_file.h"
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "p_queue.h"

//struct to simulate the CPU
typedef struct CPU{

  q *queue;

}CPU;

//struct to simulate the event queue
typedef struct event_queue{

  heap *priority_queue;
}eq;


//struct to simulate the hard disk
typedef struct I_O_Disk{

  q *queue;

}disk;

void handle_event(eq event_queue, CPU cpu);
void process_CPU(eq event_queue, event e, CPU cpu);
void process_disk1(eq event_queue, event e, CPU cpu);
void process_disk2(eq event_queue, event e, CPU cpu);
void generate_event(eq event_queue, int *values);
int count = 0;

void main(){
  eq event_queue;
  disk disk1, disk2;
  CPU cpu;
  int *values = read_file();
 
  event_queue.priority_queue = initialize();
  cpu.queue = initialize_queue();
  disk1.queue = initialize_queue();
  disk2.queue = initialize_queue();

  //start the simulation
  generate_event(event_queue, values);

}


//describes the initial state of the simulation
void generate_event(eq event_queue, int*values){
  int time = 0;
  count++;
  event SIM_START = {"SIM_START", 0, 0};
  event SIM_END = {"SIM_END", values[2], 0};
  event job1 = {"Arrival", -1, 1};
  event job2 = {"Arrival", 2, 0};
  event job3 = {"Arrival", 101, random_gen(values[3], values[4])};
  
  push(event_queue.priority_queue, SIM_START);
  push(event_queue.priority_queue, SIM_END);
  push(event_queue.priority_queue, job1);
  print_PQ(event_queue.priority_queue);
  push(event_queue.priority_queue, job2);
  push(event_queue.priority_queue, job3);

  print_PQ(event_queue.priority_queue);
  pop(event_queue.priority_queue);
  pop(event_queue.priority_queue);
  pop(event_queue.priority_queue);
  pop(event_queue.priority_queue);
  print_PQ(event_queue.priority_queue);
  
}

void handle_event(eq event_queue, CPU cpu){

  int time = 0;
  while(isEmpty(event_queue.priority_queue) && (time<10)){
    event current = pop(event_queue.priority_queue);
    time = time + current.timestamp;
    puts("the function is in the while loop");
    // switch (current.event_type){
    // case 1: process_CPU(event_queue, current, cpu); break; //Arrival
    //case 2: process_CPU(event_queue, current, cpu); break; //Finish
    
    // }
  }
}

void process_CPU(eq event_queue, event e, CPU cpu){

  
    





}
