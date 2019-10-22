
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/time.h>
#include<math.h>
#include<semaphore.h>
#include<sched.h>
#include<inttypes.h>
#include<x86intrin.h>
#include<fcntl.h>
#include<linux/input.h>
#define queue_length 10

//learning git repo 

static  inline volatile unsigned long long RDTSC(void)
{
    unsigned hi,lo;
    asm volatile ("rdtsc":"=a"(lo),"=d"(hi));    // DECLARING THE LOWER 32-BIT AND UPPER 32-BIT REGISTERS //
    return ((unsigned long long )lo)|(((unsigned long long)hi)<<32) ; // RETURNING THE PRESENT TIME-STAMP CUNTER VALUE //
}



struct message
{
    int msg_id; // MESSAGE ID //
    int src_id; // SOURCE ID //
    int destn_id; // DESTINATION ID //
    double val; // VARIABLE TO STORE THE GENERATED PI-VALUE //
    unsigned long long tsc1; // TIME-STAMP COUNTER 1 //
    unsigned long long tsc2; // TIME-STAMP COUNTER 2 //
 
};


typedef struct queue
{
    struct message *ptr_to_msg[queue_length]; // POINTER TO THE MESSAGE STRUCTURE WHICH IS OF THE DEFINED QUEUE LENGTH //
    int head; // DEFINING THE HEAD OF THE QUEUE //
    int tail; // DEFINING THE TAIL OF THE QUEUE //
    int full; // DEFINNIG THE TERM TO CHECK IF THE QUEUE IS FULL OR NOT //
    pthread_mutex_t hold; // DEFINING MUTEX TO ACHIEVE THREAD SYNCHRONIZATION //
 
}queue_t;


queue_t* sq_create()
{
    queue_t *new_queue=malloc(sizeof(queue_t)); // INITIALIZING THE QUEUE STRUCTURE PARAMETERS //
    new_queue->head=0;
    new_queue->tail=0;
    new_queue->full=0;
    pthread_mutex_init(&(new_queue->hold),NULL); // INITIALIZING THE MUTEX TO ACHIEVE SYNCHRONIZATION //
    return new_queue; // RETURNS THE QUEUE POINTER //
}
int sq_write(struct message *m1,queue_t *queue)
{
    pthread_mutex_lock(&(queue->hold)); // LOCK THE RESOURCES //
 

    if(queue->head==queue->tail && queue->full==1) // TO CHECK IF THE QUEUE IS FULL //
    {
    pthread_mutex_unlock(&(queue->hold)); //UNLOCK THE RESOURCES
    printf(" Since Queue is Full...Operation failed!!!!\n");     //SINCE THE OPERATON HAS FAILED
    return -1;
    }

    int msg_index=queue->head; // LOAD THE MESSAGE INDEX //
    queue->ptr_to_msg[msg_index]=m1;// POINT TO THE MESSAGE STRUCTURE //
    queue->head++; // INCREMENT THE HEAD //
    queue->head=(queue->head)%(queue_length); // TO MAINTAIN THE RING BUFFER WE TAKE THE MODULUS //

    if(queue->tail==queue->head)
    {
    queue->full=1; // QUEUE IS FULL //
    }
 
    queue->ptr_to_msg[msg_index]->tsc2=RDTSC(); // OBTAIN THE RDTSC VALUE ON ENQUING THE MESSAGE //
    printf("The initail time of enqueuing of MSG_ID: %d is : %llu\n",m1->msg_id,queue->ptr_to_msg[msg_index]->tsc2);
    pthread_mutex_unlock(&(queue->hold));
 
    printf("Function sq_write is called successfully\n "); // ENSURING THAT THE ENQUING WAS DONE SUCCESSFULLY//
 

    return 0;

}

int sq_read(struct message** m1,queue_t* queue)
{
    pthread_mutex_lock(&(queue->hold));
 
    if(queue->head==queue->tail && (queue->full)!=1) // CHECK IF THE QUEUE IS EMPTY //
    {
    *m1=NULL;
    pthread_mutex_unlock(&(queue->hold)); // IF THE QUEUE IS EMPTY //
    printf("QUEUE IS EMPTY...Operation failed!!!\n");     // OPERATION OF DEQUING FAILS //
    return -1;
    }
 
    int msg_index=queue->tail; // LOAD THE MESSAGE INDEX //
 
*m1=queue->ptr_to_msg[msg_index];// POINT TO  THE MESSAGE POINTER IN QUEUE STRUCTURE //

    queue->tail=(queue->tail)+1;


queue->tail=(queue->tail)%(queue_length);     // TO MAINTAIN A RING BUFFER //
 
    //printf("The before TSC1 value is %llu\n",queue->ptr_to_msg[msg_index]->tsc1);
 
    queue->ptr_to_msg[msg_index]->tsc1=(RDTSC()-(queue->ptr_to_msg[msg_index]->tsc2));// CALCULATING THE RDTSC DIFFERENCE BETWEEEN THE                                                   // ENQUING AND THE DEQUING TIME

    //printf("%llu\n",RDTSC());
    //printf("THE afterTSC1 VALUE is %llu\n",queue->ptr_to_msg[msg_index]->tsc1);

    if(queue->head==queue->tail)
    {
    queue->full=0; // ONCE THE QUEUE IS EMPTY //
    }
 
    printf("Message at receiver with source_id:%d, destination id=%d, msgid =%d, PI VALUE:%lf \n",(queue->ptr_to_msg[msg_index]->src_id),(queue->ptr_to_msg[msg_index]->destn_id),(queue->ptr_to_msg[msg_index]->msg_id),(queue->ptr_to_msg[msg_index]->val));//PRINTING THE                                                                             //RECEIVED MESSAGE INFORMATION.
         printf("FUNCTION SQ_READ CALLED SUCCESSFULLY\n");//PRINTING FOR CONFIRMATION//
 
 
     pthread_mutex_unlock(&(queue->hold)); // TO UNLOCK THE RESOURCES //
 
    return 0;

}



void sq_delete(queue_t* queue)
{
    free(queue); // DELETING THE SHARED QUEUES //
    printf("DATA QUEUE  DELETED SUCCESSFULLY\n");
}



