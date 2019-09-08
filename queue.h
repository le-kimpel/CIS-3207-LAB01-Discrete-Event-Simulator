#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
#include <stdlib.h>

//should make a standard  FIFO queue
//using a linkedlist

typedef struct jobNode{

  struct jobNode *next;
  int timestamp;
  int job_ID;

}job;


typedef struct Queue{
  job *head;
  job *tail;
  int size;
}q;


job *new_node();
void enqueue(q *queue, int timestamp, int job_ID);
job  *dequeue(q *queue);
q *initialize_queue();
#endif //QUEUE_H
