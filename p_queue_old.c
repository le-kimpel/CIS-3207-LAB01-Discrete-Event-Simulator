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

hnode * push(hnode * root, int id, int type, int time){
  // doubles size of heap if full
  if(root->size == root->max_length){
    root->max_length*=2;
    root = (hnode*)realloc(root, sizeof(hnode)*(root->max_length));
  }

  // adds item to end of heap
  root[root->size].id = id;
  root[root->size].type = type;
  root[root->size].time = time;

  // grabs final node index
  int cur_index = root->size;

  // flag to determine if swap occurred
  int flag = 0;

  // bubbles up small values inserted into heap
  while(cur_index > 1 && flag == 0){
    flag = sort_element(root, cur_index, flag);
    cur_index/=2;
    }

  // increments heap size
  root->size++;
  
  // neccessary to return root since the pointer may be reset from realloc.
  return root;
}

/*void percolate(heap *pq){ 
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
*/



// iteratively sort heap
void percolate(heap * root){
  // initializes required values for heap sort
  int smallest = 0;
  int cur_index = smallest;
  int left = 2*smallest+1;
  int right = 2*smallest+2;
  int size = root->size;
  
  // flag to check if no swap was performed
  int flag = 0;

  // swaps until end of heap or no swaps were performed
  while(flag == 0 && (smallest < size)){
    // compares upper node to its left node
    if(left < size && root->pq_arr[smallest].timestamp > root->pq_arr[left].timestamp){
      smallest = left;
    }

    // compares upper node to its right node
    if(right < size && root->pq_arr[smallest].timestamp > root->pq_arr[right].timestamp){
      smallest = right;
    }

    // swaps nodes and initializes values for next subtree in the heap
    if(smallest != cur_index){
      event temp = root->pq_arr[smallest];
      
      root->pq_arr[smallest] = root->pq_arr[cur_index];
      
      root->pq_arr[cur_index] = temp;
      
      left = 2*smallest+1;
      right = 2*smallest+2;
      cur_index = smallest;
    }else{
      // sets flag to 1 if no swaps were performed
      flag = 1;
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
