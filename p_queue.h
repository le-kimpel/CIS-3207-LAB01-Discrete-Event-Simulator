#ifndef P_QUEUE_H
#define P_QUEUE_H
#include <stdio.h>
#include <stdlib.h>


typedef struct eventNode{

  char *event_type;
  int timestamp;
  int job_ID;
}event;

typedef struct heap{
  int size; //keep track of whether pq is full or empty
  event *pq_arr; //pointers to event
  int capacity;
}heap;

//functions
heap * initialize();
void push(heap *pq, event data);
_Bool isFull(heap *pq);
_Bool isEmpty(heap *pq);
void destroy(heap *pq);
event pop(heap *pq);
void print_PQ(heap *pq);
void percolate(heap *pq);

#endif //P_QUEUE_H
