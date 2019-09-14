#include <stdio.h>
//#include "components.h"
#include "gen.h"
#include "read_file.h"
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "p_queue.h"
#include <time.h>

//values for switching between states
const int SIMULATION_START = 0;
const int JOB_ARRIVAL_SIM = 1;
const int JOB_ARRIVAL_CPU = 2;
const int JOB_LEAVES_CPU = 3;
const int JOB_ARRIVAL_DISK1 = 4;
const int JOB_LEAVES_DISK1 = 5;
const int JOB_ARRIVAL_DISK2 = 6;
const int JOB_LEAVES_DISK2 = 7;
const int CPU_BUSY = 8;
const int CPU_IDLE = 9;
const int DISK1_BUSY = 10;
const int DISK1_IDLE = 11;
const int DISK2_BUSY = 12;
const int DISK2_IDLE = 13;
const int SIMULATION_END = 14;

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
void process_CPU_enter(eq event_queue, event e, float *values, CPU *cpu, disk *disk1, disk *disk2);
void handle_event(eq event_queue, float *values, CPU *cpu, disk *disk1, disk *disk2);
void process_CPU_arrival(eq event_queue,  event e, CPU *cpu, float *values, disk *disk1, disk *disk2);
void process_CPU_finish(eq event_queue,event current, CPU *cpu, float *values, disk *disk1, disk *disk2, int DISK_PROB);
void process_disk1_arrival(eq event_queue, event e, CPU *cpu, float *values, disk *disk1, disk *disk2);
void process_disk1_finish(eq event_queue, event e, CPU *cpu, float *values, disk *disk1, disk *disk2);
void process_disk2_arrival(eq event_queue, event e, CPU *cpu, float *values, disk *disk1, disk *disk2);
void process_disk2_finish(eq event_queue, event e, CPU *cpu, float *values, disk *disk1, disk *disk2);
const int  compare_disk_queue_size(disk disk1, disk disk2, event e);



void generate_event(eq event_queue, float *values, CPU *cpu, disk *disk1,
		    disk *disk2);
int count = 0;
int global_time = 0;


void main(){
  eq event_queue;
  disk disk1, disk2;
  CPU cpu;
  float *values = read_file();

  //initialize structures and allocate memory
  event_queue.priority_queue = initialize();
  cpu.queue = initialize_queue();
  cpu.status = CPU_IDLE;
  disk1.status = DISK1_IDLE;
  disk2.status = DISK2_IDLE;
  disk1.queue = initialize_queue();
  disk2.queue = initialize_queue();


  //generate initial states
  event SIM_START = {"SIM_START",SIMULATION_START, values[1], 0};
  event SIM_END = {"SIM_END", SIMULATION_END,values[2], 0};

  push(event_queue.priority_queue, SIM_START);
  push(event_queue.priority_queue, SIM_END);

  int i = values[1];
  //generate ALL arrival events by prepopulating the queue 
  while(i < values[2]){

    event start_sim = {"Arrival_SIM", JOB_ARRIVAL_SIM, i, count++};
    i+=random_gen(values[4],values[3]);
    
    // add start sim to heap
    push(event_queue.priority_queue, start_sim);
    
  }
  //print_PQ(event_queue.priority_queue);
  
  //start the simulation
 
  //srand(time(0));
  handle_event(event_queue, values, &cpu, &disk1, &disk2);
  
}

//handles simulation arrival events and adds disk functionality
void handle_sim_arrival(eq event_queue, event e, float *values, CPU *cpu,
			disk *disk1, disk *disk2){


  process_CPU_enter(event_queue, e, values, cpu, disk1, disk2);
  

}

void process_CPU_enter(eq event_queue, event e, float *values, CPU *cpu,
		       disk *disk1, disk *disk2){

  if (cpu->status == CPU_IDLE){
        
    event cpu_arrive = {"Arrival in CPU", JOB_ARRIVAL_CPU, e.timestamp, e.job_ID};
    event cpu_finish = {"Finish in CPU", JOB_LEAVES_CPU,
			random_gen(values[7],values[6])+global_time, e.job_ID};
    push(event_queue.priority_queue, cpu_arrive);
    push(event_queue.priority_queue, cpu_finish);
    cpu->status = CPU_BUSY;
    
  }else if (cpu->status == CPU_BUSY){
    
    enqueue(cpu->queue,e.timestamp, e.job_ID);
  }else{
    puts("ERROR: event could not be enqueued");
  }
}

//handles event nodes of different types
void handle_event(eq event_queue, float *values, CPU *cpu, disk *disk1, disk *disk2){

  while(!isEmpty(event_queue.priority_queue)){

    //generate the probability of each event exiting the loop   
    int DISK_PROB = prob_select(values[5]);

    //    printf("**************\n");
    //print_PQ(event_queue.priority_queue);
   
    //pop the minimum node from the event queue
    event current = pop(event_queue.priority_queue);
    
    global_time = current.timestamp;  
    if (current.event_num == 1){
      printf("%s%d%s%d\n", "job", current.job_ID,
	     " arrived in simulation at time ", current.timestamp); 
    } else if (current.event_num == 2){
      printf("%s%d%s%d\n", "job", current.job_ID,
	     " arrived in CPU at time ", current.timestamp);
    }else if (current.event_num == 3){
      printf("%s%d%s%d\n", "job", current.job_ID,
	     " left CPU at time ", current.timestamp);
    }else if (current.event_num == 4){
      printf("%s%d%s%d\n", "job", current.job_ID,
	     " arrived in Disk1 at time ", current.timestamp);
    }else if (current.event_num == 5){
      printf("%s%d%s%d\n", "job", current.job_ID,
	     " left Disk1 at time ", current.timestamp);
    }else if (current.event_num == 6){
      printf("%s%d%s%d\n", "job",current.job_ID,
	     " arrived in Disk2 at time ", current.timestamp);
    }else if(current.event_num == 7){
      printf("%s%d%s%d\n", "job",current.job_ID,
	     " left Disk2 at time ", current.timestamp);
    }else if(current.event_num == SIMULATION_END){
      printf("Simulation ended\n");
      exit(0);
    }
   
    switch (current.event_num){
    case 1:
      handle_sim_arrival(event_queue, current, values,cpu, disk1, disk2);
      break;
    case 3 :
      process_CPU_finish(event_queue, current, cpu, values, disk1, disk2,
			 DISK_PROB); 
      break;
     case 5:
       process_disk1_finish(event_queue, current, cpu, values, disk1,
			    disk2);
       break;
      case 7:
	process_disk2_finish(event_queue, current, cpu, values, disk1,
			     disk2);
	break;
    case 14 :  break; //SIM_END
    }
 }
}

/*
 the new arrival job will be marked with an event determining
its finishing time and enqueued in the event queue.
This means that, when these event is popped, it will return to this method,
and a message will be generated.*/


/*when an event is finished in the cpu, there is a chance 
  it may exit  the simulation.*/
void process_CPU_finish(eq event_queue, event e, CPU *cpu, float *values, disk *disk1, disk *disk2, int DISK_PROB){
    //events marked as leaving CPU go here to be marked for processing by disk(s)

    cpu->status = CPU_IDLE;
   
    //if QUIT_PROB condition is met
    //compare disk sizes and send to the appropriate queue  
 
    if (DISK_PROB == 0) {
      //job continues in simulation
      
      if (disk1->status == DISK1_IDLE){
	//send directly to disk1
	event disk1_to_arrive = {"Arrival in Disk1", JOB_ARRIVAL_DISK1,
				 global_time, e.job_ID};
        event disk1_to_finish = {"Finish in Disk1", JOB_LEAVES_DISK1,
				 random_gen(values[9],values[8])+global_time,
				 e.job_ID};
	push(event_queue.priority_queue, disk1_to_arrive);
	push(event_queue.priority_queue, disk1_to_finish);
	disk1->status = DISK1_BUSY;

      }else if (disk2->status == DISK2_IDLE){
	//send directly to disk2

	event disk2_to_arrive = {"Arrival in Disk2", JOB_ARRIVAL_DISK2,
				 global_time, e.job_ID};
	event disk2_to_finish = {"Finish in Disk2", JOB_LEAVES_DISK2,
				 random_gen(values[9],values[8])+global_time,
				 e.job_ID};

	push(event_queue.priority_queue, disk2_to_arrive);
	push(event_queue.priority_queue, disk2_to_finish);
	disk2->status = DISK2_BUSY;
      
    	 //compare FIFO queue sizes of Disk 1 and Disk 2
	//if they are equal, choose at random
      }else{
	if(compare_disk_queue_size(*disk1, *disk2, e) == JOB_ARRIVAL_DISK1){
	  enqueue(disk1->queue,e.timestamp, e.job_ID);

	}else{
	  enqueue(disk2->queue,e.timestamp,e.job_ID);
	}
      }
    }else{
      printf("%s%d%s\n", "JOB " , e.job_ID, " LEFT THE SIMULATION");
    }
    
    //dequeue the events waiting in the queue and send them to event handler
    //to be immediately processed in CPU
    if (!QisEmpty(cpu->queue)){
      job *task = dequeue(cpu->queue);

      event to_arrive = {"Arrive in CPU", JOB_ARRIVAL_CPU, global_time,
			 task->job_ID};
      event to_finish = {"Finish in CPU", JOB_LEAVES_CPU,
			 random_gen(values[7],values[6])+global_time, task->job_ID};

      push(event_queue.priority_queue, to_arrive);
      push(event_queue.priority_queue, to_finish);
      
      cpu->status = CPU_BUSY;
    }
}

//compares the size of each FIFO queue in disks
//returns either JOB_ARRIVAL_DISK1 or JOB_ARRIVAL_DISK2
const int compare_disk_queue_size(disk disk1, disk disk2, event e){

  if (disk1.queue->size > disk2.queue->size){
   
    return JOB_ARRIVAL_DISK2;
  }else if (disk2.queue->size > disk1.queue->size){
    return JOB_ARRIVAL_DISK1;

  }else if (disk1.queue->size == disk2.queue->size){
    
    int result = rand()%2;
    if (result == 1){
      return JOB_ARRIVAL_DISK1;
    }else{
      return JOB_ARRIVAL_DISK2;
    }
  }
}

void process_disk1_finish(eq event_queue, event e, CPU *cpu, float *values, disk *disk1, disk *disk2){

  //set disk1 status to idle, as event has finished in disk1
    disk1->status = DISK1_IDLE;


    //checks CPU if it's empty. If it is, sends arrival event straight into CPU
    process_CPU_enter(event_queue, e, values, cpu,  disk1, disk2);



    if (!QisEmpty(disk1->queue)){

      job *task = dequeue(disk1->queue);

      event to_arrive_d1 = {"Disk1 read arrival", JOB_ARRIVAL_DISK1, global_time,
			    task->job_ID};
      event to_finish_d1 = {"Disk1 read finished", JOB_LEAVES_DISK1,
			    random_gen(values[9],values[8])+global_time,
			    task->job_ID};

      push(event_queue.priority_queue, to_arrive_d1);
      push(event_queue.priority_queue, to_finish_d1);
      
      disk1->status = DISK1_BUSY;
    }
  }

void process_disk2_finish(eq event_queue, event e, CPU *cpu, float *values, disk *disk1, disk *disk2){


     disk2->status = DISK2_IDLE;
      
     process_CPU_enter(event_queue, e, values, cpu, disk1, disk2);
    
    if (!QisEmpty(disk2->queue)){

      job *task = dequeue(disk2->queue);

      event to_arrive_d2 = {"Disk2 read arrival", JOB_ARRIVAL_DISK2, global_time,
			    task->job_ID};
      event to_finish_d2 = {"Finished at disk 2", JOB_LEAVES_DISK2,
			    random_gen(values[11],values[10])+global_time,
			    task->job_ID};

      push(event_queue.priority_queue, to_arrive_d2);
      push(event_queue.priority_queue, to_finish_d2);
	
      disk2->status = DISK2_BUSY;
    }
    
}
