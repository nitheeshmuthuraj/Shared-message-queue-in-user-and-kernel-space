# Shared Message Queues in User and Kernel Space

Message  bus  is  a  useful  middleware  to  support  the  communication  among  user  applications. The core  component  of  a  message  bus  is  its  bus  daemon  which  manages  connections  and  forwards messages through  message  queues.


There are two queues implemented with circular buffer concept with the size being 10 perqueue .Each queue can accept 2 periodic sender & aperiodic sender(mouse click event) . 

The Periodic threads would be enqueued to respective queue according to the predefined priority & the multiplier constant.

On click of the mouse a new thread would be generated which would be enqueued

The mouse click have a higher priority compared to the periodic events which can be obsereved in kernel tracer.

On Observing successive Double click events within 500 ms the program exits.

Mutex locks were used for the process of enqueueing &  Dequeueing& the time difference between Enqueue & Dequeue.

The  threads  are  scheduled with Real-time FIFO policy.


## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.


### Installing GCC compiler 

sudo apt-get install gcc - this would install gcc compiler on hostmachine


#### Running the tests

1) make all -- The program will begin execution (annexure b and c given below will be executed)

2) Click the left mouse buttons multiple times followed by 2 successive click which would exit the program

3) kernelshark trace.dat  -- this command would display the threads schedule times.


#####Apply filters as below:

Plots->CPUs-> check only cpu0
Plots->Task->check only a.out threads (there are 9 such threads)

######## Annexure 
a) gedit main.c -- instruction to access the file contents of main.c

b) gcc main.c queue.c -lm -lpthread --this would compile & generate a.out file which is an object file -lm & -lpthread are used for linking the respective libraries 

c) sudo trace-cmd record -e sched_switch taskset 0x01 ./a.out -- this function will execute the object file ,record the trace after setting the 									affinity to single core

## Authors

NITHEESH MUTHURAJ 
ASU ID:1213383988
nitheesh@asu.edu
 
REVANTH RAJSHEKAR 
ASU ID:1213380816
rmrajash@asu.edu

