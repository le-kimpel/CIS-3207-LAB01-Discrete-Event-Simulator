#include <stdio.h>
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


