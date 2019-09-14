#include <stdio.h>
#include <stdlib.h>


typedef struct eventNode{

  char *event_type; //string to dictate the type in debugging
  int event_num; //constant type
  int timestamp; //timestamp of event
  int job_ID; //job ID
}event;

typedef struct heap{
  int size; //keep track of whether pq is full or empty
  event *pq_arr; //pointers to event
  int capacity;//storage capacity
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
int sort_pq(heap * pq, int level, int swap_switch);

//initializes the priority queue
heap *initialize(){
  heap *pq = (heap*)malloc(sizeof(heap));
  pq->size = 0;
  pq->capacity = 10;
  pq->pq_arr = (event*)malloc(sizeof(event)*pq->capacity);
  return pq;
}

//removes the top (min) element
event pop(heap *pq){

  event popped = pq->pq_arr[0];

  // swap first and last nodes
  if (!isEmpty(pq)){
    pq->pq_arr[0] = pq->pq_arr[pq->size-1];
    pq->size--;
    percolate(pq);
    
  }

  return popped;
}
  
//pushes an element into the priority queue
void push(heap *pq, event data){
  // doubles size if full
  isFull(pq);

  // adds item to end of heap
  pq->pq_arr[pq->size] = data;

  // grabs final node index
  int cur_index = pq->size;

  // switch for swapping nodes
  int swap_switch = 0;

  // sorts the values
  while(cur_index > 0 && swap_switch == 0){
    swap_switch = sort_pq(pq, cur_index, swap_switch);
    cur_index/=2;
  }

  pq->size++;
}

int sort_pq(heap * pq, int level, int swap_switch){
 
  int new_level = level/2;

  // swaps nodes if lower node is less than higher node
  if(pq->pq_arr[level].timestamp < pq->pq_arr[new_level].timestamp){
    event temp = pq->pq_arr[level];
    
    pq->pq_arr[level] = pq->pq_arr[new_level];
    
    pq->pq_arr[new_level] = temp;
  }else{
    // sets switch  to 1 if nodes were not swapped
    swap_switch = 1;
  }


  return swap_switch;
}

void percolate(heap * pq){
  // initializes heapsort base values
  int smallest = 0;
  int cur_index = smallest;
  int left = 2*smallest+1;
  int right = 2*smallest+2;
  int size = pq->size;
  
  int swap_switch = 0;

 
  while(swap_switch == 0 && (smallest < size)){
    //comparing upper nodes to the leftmost nodes
    if(left < size && pq->pq_arr[smallest].timestamp > pq->pq_arr[left].timestamp){
      smallest = left;
    }

    // comparing upper nodes to its rightmost nodes
    if(right < size && pq->pq_arr[smallest].timestamp > pq->pq_arr[right].timestamp){
      smallest = right;
    }

    // swaps nodes
    if(smallest != cur_index){
      event temp = pq->pq_arr[smallest];
      
      pq->pq_arr[smallest] = pq->pq_arr[cur_index];
      
      pq->pq_arr[cur_index] = temp;
      
      left = 2*smallest+1;
      right = 2*smallest+2;
      cur_index = smallest;
    }else{
      // no swaps
      swap_switch = 1;
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

//prints the queue
void print_PQ(heap *pq){
  puts("");
  printf("%s", "[");
  for (int i = 0; i<pq->size; i++){
    printf("%d%s%d%s%s%s", pq->pq_arr[i].timestamp, ",", pq->pq_arr[i].job_ID, ",", pq->pq_arr[i].event_type, "||");
  }
  printf("%s","]");
  puts("");
 

}
