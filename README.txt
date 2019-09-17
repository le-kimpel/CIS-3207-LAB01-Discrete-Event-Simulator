-------------------------
CIS 3207 README.txt LAB01
-------------------------

NAME: Lauren Kimpel
tuID: tug60017@temple.edu
LAB: W 1-2:50

-------------------------

TABLE OF CONTENTS

(0) - Overview
(1) - Libraries and Constants
(2) - Program Workflow by Function
(3) - Conclusion

OVERVIEW

The purpose of this program is to construct and test a hypothetical CPU scheduler.
The function of the scheduler relies on several key data structures:
3 FIFO queues, implemented to resemble two disks and a CPU, and one min-heap, designated
as an event queue. Output from the scheduler depends on the values assigned to the events moving
between these structures in CONFIG.txt. Overall, the architecture of the mechanism is intended
to be FIFO - or, first-in-first-out, meaning that events with earlier timestamps are serviced
much more immediately than those that arrive late.
"Jobs" - or, serviced events - have a total of 3-4 actions that they perform when cycling
through the scheduler. These are:

	JOB_ARRIVAL: a job arrives DIRECTLY in a component  

	JOB_LEAVE: a job finishes at a component

	JOB_SITS_IN_FIFO_QUEUE: a job is ENQUEUED in a component's FIFO queue, as a result of the
	component being set to BUSY
	
	JOB_EXIT_SIMULATION: a job does not continue to the disks. 

The implemented program uses a total of 8 functions and 3 C struct types: CPU, event_queue, and
I_O_Disk. It relies on three key global variables: count (for initializing and incrementing
job ID's); global_time (for keeping track of timestamps); and fp, a FILE pointer to write to log.txt the output.

    (1) CONSTANT VARIABLES and LIBRARIES - "main.c"

In main.c, the following libraries are included in order for the program to run:
stdio.h - for I/O and debuggging
stdlib.h - to enable memory allocation and generation of random variables
string.h - to concatenate strings if necessary for output purposes
time.h - to seed a random for debugging

This program also uses custom libraries built to more easily access extra functionality,
this functionality being: as simple random generator, the probability generator that relies on it,
and all data structure methods, as well as a .h file for reading from CONFIG.txt.

"gen.h" - the source for nearly all random methods. These methods include:
1)	int random_gen(int max, int min)
2)	_Bool prob_select(float percentage)

1) random_gen(int max, int min) takes a maximum and minimum integer and calculates a random
value between them. This is crucial for calculating timestamp values in finishing jobs.

2) prob_select(float percentage) takes a float for its percentage calculation and manufactures
a probability based on this float. It uses random_gen to find a number between percentage
and 100. This is based in deciding what jobs go to disk, and what jobs leave the simulation.

"read_file.h" - .h file used to read from a text file specific floats in order to implement
config.txt properly. It searches for tagged words in a string of text, and copies them into
float *values for frequent use within the program.

"p_queue".h - .h file used to create the priority queue data structure and its underlying heap.
Functions include:

1) heap *initialize();
2) void push(heap *pq, event data);
3) _Bool isFull(heap *pq);
4) _Bool isEmpty(heap *pq);
5) void destroy(heap *pq);
6) event pop(heap *pq);
7) void print_PQ(heap *pq);
8) void percolate(heap *pq);
9) int sort_pq(heap *pq, int level, int swap_switch);

structs include:

typedef struct heap;
typedef struct eventNode;

The heap for the priority queue was implemented using an ArrayList. This was heavily preferred
over a LinkedList, due to ease of traversal. A percolate() or heapify() method, when called on
a heap with a LinkedList implementation, would perform significantly worse than that of an
ArrayList, whose traversal time is much quicker and easier to implement than that of a LinkedList.
Even though pushing to a LinkedList would be faster and more convenient, the resulting
sort that would have to occur saps much of the efficiency from the heap. 

"queue.h" is the source of all FIFO activity. Functions include:

1) _Bool QisEmpty(q *queue);
2) job *new_node();
3) void enqueue(q *queue, int timestamp, int job_ID);
4) job *dequeue(q *queue);
5) q *initialize_queue();

structs include:

typedef struct jobNode;
typedef struct Queue;

Again, we are presented with a choice for FIFO queue underlying data structure, the most sensible
of which is with a LinkedList rather than an ArrayList. Unlike the heap, which has the added
functionality of heapsort(), there is no sorting in a FIFO queue: first job in gets serviced first.
Thus, we really only need a structure that can add and remove easily, not requiring insertions
or expensive tree traversals. Thus, the structure for this program's FIFO queue(s) is made with
a generic LinkedList in C.

The CONSANT VARIABLES used in this program are as follows:

const int SIMULATION_START;
const int JOB_ARRIVAL_SIM;
const int JOB_ARRIVAL_CPU;
const int JOB_LEAVES_CPU;
const int JOB_ARRIVAL_DISK1;
const int JOB_LEAVES_DISK1;
const int JOB_ARRIVAL_DISK2;
const int JOB_LEAVES_DISK2;
const int CPU_BUSY;
const int CPU_IDLE;
const int DISK1_BUSY;
const int DISK1_IDLE;
const int DISK2_BUSY;
const int DISK2_IDLE;
const int SIMULATION_END;

These are used as flags to determine what actions are taken by the scheduler depending on which
events are popped, which events are dequeued, the occupation of the CPU/Disks, and the start
and end time of the entire simulation. They are all initialized to a set of increasing constant
ints ranging from 0-14. 

     (2) WORKFLOW OF THE PROGRAM BY FUNCTION

The design of this section will go through the program by method in the order that they are
listed in the main.c file.

-----------
void main()
-----------
Main() is where ALL data structures are initialized and memory allocated, where

eq event_queue is the PRIORITY QUEUE.
disk disk1, disk2 are the I/O DISKS.
CPU cpu is the CPU.
float *values are all the read-in values from CONFIG.txt.

The program starts by printing values[0]-values[11] to keep track of config values.
We then initialize our structures; our CPU, Disk1, and Disk2 are all set to IDLE.

TWO events are generated once we start:

event SIM_START = {"SIM_START", SIMULATION_START, values[1], 0};
event SIM_END = {"SIM_END", SIMULATION_END, values[2], 0};

The contents of an "event" according to this implementation of the program are as follows.

event e = {char event_type, int event_num, int timestamp, int job_ID}, where
event_type is the string represetation of the event, used for tracking and debugging;
event_num is the phyiscal flag for what an event actually is performing (arrival, finish, etc.);
job_ID is the job ID.

The start and end simulation events are pushed into the queue using the heap's push() function.

One significant detail that occurs at the start of the simulation is this: ARRIVAL events are
preloaded into the event queue. This means to resolve any potential timing descrepancies between
arrivals and finishes, and seeks to massively simplify how events are generated.
This is achieved through a while loop that seeks to generate one event per iteration, which lasts
until the FIN_TIME config value is reached. These events are then immediately pushed into the queue
with corresponding random timestamps and incrementing job IDs, so that they remain in order.
The alternative was to generate one CPU arrival event after each finishing CPU event.
However, this presented a few problems, the first being that there would be no way to handle
events constantly entering the simulation and going straight into the CPU rather than into the
queue. This also helps to make the simulation more predictable and reduces undefined behavior
(timestamp is overwritten, multiple jobs enter CPU at the same time, jobs finish out of order.)

--------------------------------------------------------------------------------------------------
void handle_sim_arrival(eq event_queue, event e, float *values, CPU *cpu, disk *disk1, disk *disk2);
---------------------------------------------------------------------------------------------------
This function is one line. All this does is simulate the processing of a new event into the
simulation. This function calls handle_event() to trigger the scheduler.

-------------------------------------------------------------------------------------
void handle_event(eq event_queue, float *values, CPU *cpu, disk *disk1, disk *disk2)
-------------------------------------------------------------------------------------
handle_event() consists of a while loop that terminates either when the priority queue is empty
(in other words, never), or when the exit(0) call is issued. This function prints statistics
as well as events that have been immediately popped from the event queue. The switch statement
towards the bottom of the handler manages the popped event's event_num. So, an event is popped,
then logged to a file, then handled elswhere in the program depending on its tag.

-------------------------------------------------------------------------------------------------
void process_CPU_enter(eq event_queue, event e, float *values, CPU *cpu, disk *disk1, disk *disk2);
-------------------------------------------------------------------------------------------------
This function first checks the status of the CPU before doing anything. This is because multiple
events are attempting to access the CPU (those leaving the disks and those entering the
simulation.) If the CPU's status is set to CPU_IDLE, then TWO events will be generated: cpu_arrive
and cpu_finish. Stats are also calculated in this function.
If the CPU is set to CPU_BUSY, events do NOT immediately go into the CPU, but instead are enqueued
along with a timestamp and job ID. If none of these conditions is satisfied, it prints an
error message.


-------------------------------------------------------------------------------------------------
void process_CPU_finish(eq event_queue, event e, float *values, CPU *cpu, disk *disk1, disk *disk2);
-------------------------------------------------------------------------------------------------
This function takes events scheduled to leave the CPU and diverts them:

1) out of the simulation entirely, or
2) to disk1 or disk2, depending on the size of the FIFO queue, or
3) if enqueued and waiting for service, this function can dequeue them.

First, the CPU's status is reset to CPU_IDLE, to simulate a job leaving the CPU.
If the calculated disk leave probability is 0 (in other words, it does NOT leave):

*Note: this function uses the function int compare_disk_queue_size().
The function first checks Disk1's status. This implies that the program has a slight trivial
preference for Disk1, since it checks this queue first, and every job that does not leave
the simulation will practically be forced here. Then, it checks Disk2's status. It generates
the corresponding arrival and finish events based on the FIFO queue size and whether a job can
be immediately inserted into each disk. 

Otherwise, if jobs are waiting in the CPU's queue, it dequeues them and sends them back as arrival
and finish events. Then, the CPU is set to CPU_BUSY. Note that only one event can be occupied
in each component.

All other finish methods are exactly the same as process_CPU finish, albeit with lesser
functionality. The disks are merely replicated from the CPU methods. The CPU methods
have the added responsibility of handling many events resuming in the cycle, so they were
designed with this in mind.


	 (3) - CONCLUSION

There were many ways to implement this program. One such way, initially tested, was to have events
generating in increments at the end of process_CPU_finish. I initially had a process_CPU_arrival
function and its Disk1 and Disk2 equivalents, but learned that this needlessly complicated the
design and put limitations on the program as well as generated incorrect times and therefore
out-of-order jobs.
The main problem in this lab was determining what an "event" was.
Is an arrival immediate or is it also when an event is queued for arrival? In this program,
an arrival is IMMEDIATE. An event does NOT arrive until it is dequeued.
Additionally, debugging skills improved, out of necessity on part of the heap being a hidden
thorn in my side. Data structures come FIRST and need to be rigorously tested in order to prevent
an infinite tide of logic errors. The first week of the lab was wasted chasing bugs
that wouldn't have existed, had I checked whether my heap worked correctly.

