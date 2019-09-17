#include <stdio.h>
#include <stdlib.h>

//should make a standard  FIFO queue
//using a linkedlist

//nodes used for the queue
typedef struct jobNode{

struct jobNode *next;
  int timestamp;
  int job_ID;

}job;

//the queue structure: one node for the head, and one for the tail
typedef struct Queue{
  job *head;
  job *tail;
  int size;
}q;

//function prototypes
_Bool QisEmpty(q *queue);
job *new_node();
void enqueue(q *queue, int timestamp, int job_ID);
job  *dequeue(q *queue);
q *initialize_queue();

q *initialize_queue(){
  
q *queue = (q*)malloc(sizeof(q));
  queue->head = NULL;
  queue->tail = NULL;
  queue->size = 0;
  return queue;
}

//adds a new node by allocating memory node-by-node
//this is NOT like the priority queue, which adds twice as much memory
job *new_node(int timestamp, int job_ID){
  
  job *temp = (job*)malloc(sizeof(job));
  temp->timestamp = timestamp;
  temp->job_ID = job_ID;
  temp->next = NULL;
  return temp;
  
}

//adds to the end of the linkedlist
void enqueue(q *queue, int timestamp, int job_ID){

  job *temp = new_node(timestamp, job_ID);
  
  if (queue->tail == NULL || queue->size == 0){
    queue->head = temp;
    queue->tail = temp;
  }else{

    queue->tail->next = temp;
    queue->tail = temp;

  }
  queue->size++;
}                                            

//removes from the front end of the linked list
job *dequeue(q *queue){
 
  if (queue->size == 0){
    return NULL;
  }else{

    job *temp = queue->head;
    queue->head = queue->head->next;
    queue->size--;

    return temp;
  }
  }
//checks whether the queue has nodes in it
  _Bool QisEmpty(q *queue){
    if (queue->size == 0){
      return 1;
    }else{
      return 0;
    }
}
