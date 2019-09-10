#include <stdio.h>
#include <stdlib.h>


typedef struct eventNode{

  char *event_type;
  int event_num;
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


//initializes the priority queue
heap *initialize(){
  heap *pq = (heap*)malloc(sizeof(heap));
  pq->size = 0;
  pq->capacity = 10;
return pq;
}

//removes the top (min) element
event pop(heap *pq){

  event popped = pq->pq_arr[0];
  
  if (!isEmpty(pq)){
    pq->pq_arr[0] = pq->pq_arr[pq->size-1];
    pq->size--;
    percolate(pq);
    
  }

  return popped;
}
  
//pushes an element into the priority queue
void push(heap *pq, event data){

  if (isEmpty(pq)){
    
    pq->pq_arr = (event*)malloc(sizeof(event)*2*pq->capacity);
    pq->pq_arr[0] = data; //inserts the data at the root if empty
    pq->size++;
  }

  else {
   
    
    isFull(pq);
    pq->pq_arr[pq->size] = data;
    pq->size++;
    percolate(pq);
  }
}

 void percolate(heap *pq){ 
   //percolates up upon insertion at the end of the queue
   event temp;

   for (int i = pq->size-1; i>=0; i/=2){
     
     event current = pq->pq_arr[i];
       
     if (current.timestamp < pq->pq_arr[i/2].timestamp){
     
       temp = pq->pq_arr[i/2];

       pq->pq_arr[i/2] = current;

       pq->pq_arr[i] = temp;
     }else{
       break;
     }
   }
 
  }
 
  
//checks if heap is full
_Bool isFull(heap *pq){
  if (pq->size>=pq->capacity){
    pq->capacity *=2;
    pq->pq_arr = (event*)realloc(pq->pq_arr, sizeof(event)*2*pq->capacity);
    return 1;
  }else{
    return 0;
  }
}

//checks if heap is empty
_Bool isEmpty(heap *pq){
  if (pq->size == 0){
    return 1;
  }else{
    return 0;
  }
}

//frees allocated memory
void destroy(heap*pq){
  pq = NULL;
  free(pq);
}

void print_PQ(heap *pq){
  puts("");
  printf("%s", "[");
  for (int i = 0; i<pq->size; i++){
    printf("%d%s%d%s%s%s", pq->pq_arr[i].timestamp, ",", pq->pq_arr[i].job_ID, ",", pq->pq_arr[i].event_type, "||");
  }
  printf("%s","]");
  puts("");
 

}
