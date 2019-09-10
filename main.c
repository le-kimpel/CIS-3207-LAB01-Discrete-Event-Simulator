#include <stdio.h>
//#include "components.h"
#include "gen.h"
#include "read_file.h"
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "p_queue.h"

const int SIMULATION_START = 0;
const int JOB_ARRIVAL_SIM = 1;
const int JOB_ARRIVAL_CPU = 2;
const int JOB_LEAVES_CPU = 3;
const int JOB_ARRIVAL_DISK1 = 4;
const int JOB_LEAVES_DISK1 = 5;
const int JOB_ARRIVAL_DISK2 = 6;
const int JOB_LEAVES_DISK2 = 7;
const int CPU_BUSY = 8;
const int DISK1_BUSY = 9;
const int DISK2_BUSY = 10;
const int SIMULATION_END = 11;

//struct to simulate the CPU
typedef struct CPU{

  q *queue;
  int status;
  
}CPU;

//struct to simulate the event queue
typedef struct event_queue{

  heap *priority_queue;
}eq;


//struct to simulate the hard disk
typedef struct I_O_Disk{

  q *queue;
  int status;

}disk;

void handle_event(eq event_queue, int *values, CPU cpu);
void process_CPU(eq event_queue,  CPU cpu, int *values);
void process_sim_arrival(eq event_queue, CPU cpu, int *values);
void process_disk1(eq event_queue, event e, CPU cpu, int *values);
void process_disk2(eq event_queue, event e, CPU cpu, int *values);
void generate_event(eq event_queue, int *values, CPU cpu);
int count = 0;
int global_time = 0;

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
  generate_event(event_queue, values, cpu);

}


//describes the initial state of the simulation
void generate_event(eq event_queue, int*values, CPU cpu){
  
  count++;
  event SIM_START = {"SIM_START",SIMULATION_START, 0, 0};
  event SIM_END = {"SIM_END", SIMULATION_END,values[2], 0};
  event job1 = {"Arrival", JOB_ARRIVAL_SIM,random_gen(values[3],values[4]),count};
  //event job2 = {"Arrival", 2, 0};
  // event job3 = {"Arrival", 101, random_gen(values[3], values[4])};

 
  push(event_queue.priority_queue, SIM_START);
  push(event_queue.priority_queue, SIM_END);
  push(event_queue.priority_queue, job1);

  print_PQ(event_queue.priority_queue);

  handle_event(event_queue, values, cpu);
  
}

//handles event nodes of different types
void handle_event(eq event_queue, int *values, CPU cpu){

  
  while(!isEmpty(event_queue.priority_queue) && global_time <10){
   event current = pop(event_queue.priority_queue);
  global_time = global_time + current.timestamp;
    
    switch (current.event_num){

    case 1: process_sim_arrival(event_queue, cpu, values); break; //Arrival in sim
    case 2: process_CPU(event_queue, cpu, values); break; //Arrival in CPU
    case 3 : process_CPU(event_queue, cpu, values); break; //Finish in CPU
    
    case 10 : break; //SIM_END
      
    }

    print_PQ(event_queue.priority_queue);
  }
}

//enqueue the event in the FIFO queue relative to the CPU
//now the event should be CPU_ARRIVAL.
//the ARRIVAL event is returned to the priority queue.
void process_sim_arrival(eq event_queue, CPU cpu, int *values){  
  //create FIFO task
  //set CPU status to busy
  //push new event into event queue

  if (JOB_ARRIVAL_SIM){

    
   event arrive_sim = {"ARRIVED IN SIMULATION", JOB_ARRIVAL_SIM, random_gen(values[3],values[4]), count}; 
   printf("%s%d%s%d\n", "job", arrive_sim.job_ID, " arrived in the simulation at time ", arrive_sim.timestamp);
   //push(event_queue.priority_queue, CPU_arrival);

  }

}


void process_CPU(eq event_queue, CPU cpu, int *values){



}
