
int rx_count=0;



queue_t* sq_create();
int  sq_write(struct message*,queue_t*);
int  sq_read(struct message **,queue_t*);
void sq_delete(queue_t*);

void *sender_function(void *ptr);
void *receiver_function(void *ptr);


queue_t* sq_create()
{
    queue_t *new_queue=malloc(sizeof(queue_t));
    new_queue->head=0;
    new_queue->tail=0;
    new_queue->full=0;
    pthread_mutex_init(&(new_queue->hold),NULL);
    return new_queue;
}
 


//DECLARING SHARED QUEUES//
 queue_t* bus_in_q1;
 queue_t* bus_in_q2;



//ENQUEING//

int sq_write(struct message *m1,queue_t *queue)
{
    pthread_mutex_lock(&(queue->hold));
   

    if(queue->head==queue->tail && queue->full==1)
    {
    pthread_mutex_unlock(&(queue->hold));
    printf("Operation failed!!!!\n");
    return -1;
    }

    int msg_index=queue->head;
    queue->ptr_to_msg[msg_index]=m1;
    queue->head++;
    queue->head=(queue->head)%(queue_length);

    if(queue->tail==queue->head)
    {
    queue->full=1;
    }

    pthread_mutex_unlock(&(queue->hold));

    printf("Function sq_write is called successfully\n ");

    return 0;

}


//DEQUEUING//
int sq_read(struct message** m1,queue_t* queue)
{
    pthread_mutex_lock(&(queue->hold));
   
    if(queue->head==queue->tail && (queue->full)!=1)
    {
    *m1=NULL;
    pthread_mutex_unlock(&(queue->hold));
    printf("Operation failed!!!!\n");
    return -1;
    }

    int msg_index=queue->tail;
    *m1=queue->ptr_to_msg[msg_index];
    (queue->tail)++;
    queue->tail=queue->tail%queue_length;

    if(queue->head==queue->tail)
    {
    queue->full=0;
    }
   
    printf("Message at receiver with source_id:%d, destination id=%d, msgid =%d, PI VALUE:%lf \n",(queue->ptr_to_msg[msg_index]->src_id),          (queue->ptr_to_msg[msg_index]->destn_id),(queue->ptr_to_msg[msg_index]->msg_id),(queue->ptr_to_msg[msg_index]->val));


    pthread_mutex_unlock(&(queue->hold));

    printf("FUNCTION SQ_READ CALLED SUCCESSFULLY\n");
   
   
    return 0;

}

void sq_delete(queue_t* queue)
{
    free(queue);
    printf("FUNCTION SQ_DELETE CALLED SUCCESSFULLY\n");
}


pthread_mutex_t hold=PTHREAD_MUTEX_INITIALIZER;


//SENDING THREADS//

void *sender_function(void *ptr)
{
    struct param new=*(struct param *)ptr;
    int source_id;
    source_id=*(new.a);
   
    sem_t s_act;
    int k;
        double random,n;
        double i, j;     // Number of iterations and control variables
        double f;           // factor that repeats
        double pi;
   


    printf("The passed multiplier is : %d \n",new.b);
   
    struct timespec next,period;
    sem_init(&s_act,0,1);
    sem_wait(&(s_act));
    period.tv_nsec=new.b*BASE_PERIOD*1000;
   
    clock_gettime(CLOCK_MONOTONIC,&next);

   
    while(count<message_length)
    {

   
        struct message *m1;
        m1=(struct message *)malloc(sizeof(struct message));
   
        m1->src_id=source_id;
        printf("SOURCE_ID: %d\t",m1->src_id);
        if(source_id==0 || source_id==1 ||source_id==2)
        {    m1->destn_id=0;
            printf("DESTINATION_ID: %d\t",m1->destn_id);
        }
   
   
        if(source_id==3 || source_id==4 ||source_id==5)
        {    m1->destn_id=1;
            printf("DESTINATION_ID: %d\t",m1->destn_id);
        }
        pthread_mutex_lock (&hold);
               m1->msg_id=++count;
        printf("MSG_ID: %d\t\n",count);
        pthread_mutex_unlock (&hold);
   

    pi = 1;
    random=rand()%51;
       if(random<=10)
       {
    random+=11;
       }
       else;
 
     
       n=random;
     printf("The random number generated is : %lf\n",n);
 
   
       for(i = n; i > 1; i--) {
        f = 2;
        for(j = 1; j < i; j++){
        f = 2 + sqrt(f);
        }
        f = sqrt(f);
        pi = pi * f / 2;
        }
        pi *= sqrt(2) / 2;
        pi = 2 / pi;
  
        printf("Aproximated value of PI = %1.16lf\n", pi);


    m1->val=pi;

    printf("SENDING MESSAGE_ID IS : %d\t \n",m1->msg_id);

    if(m1->destn_id==0)
        {k=sq_write(m1,bus_in_q1);
         printf("===================================================================\n");
        }
    if(m1->destn_id==1)
        {k=sq_write(m1,bus_in_q2);
             printf("===================================================================\n");
        }


    next.tv_nsec=next.tv_nsec+period.tv_nsec;
    clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&next,NULL);
   

   
    }
    sem_post(&(s_act));
   
    pthread_exit(0);
   
    return NULL;

}



//RECEIVING THREADS//

void *receiver_function(void *ptr)

{   
    struct param new=*(struct param *)ptr;
    sem_t s_act;
    int receiver_id;
    int k;
    struct timespec next,period;
   
    sem_init(&s_act,0,1);
    sem_wait(&s_act);
    printf("The received multiplier is : %d\n",new.b);
    period.tv_nsec=new.b*BASE_PERIOD*1000;
   
    clock_gettime(CLOCK_MONOTONIC,&next);
   
     while(count<message_length)
    {   
        struct message *m1=malloc(sizeof(struct message));
        receiver_id=*(new.a);
        receiver_id=receiver_id+(rand()%2);
        printf("\n======================================================\n");

        if(flag==0)
        {    k=sq_read(&m1,bus_in_q1);
            if(k==0)
                printf("BUS_IN_Q1 IS BEING DE-QUEUED\n");
             pthread_mutex_lock(&hold);
                    rx_count++;
             printf("The no. of message received is : %d \n",rx_count);
             printf("====================================================================================\n");
             flag=1;
             pthread_mutex_unlock(&hold);
        }
   
        else if(flag==1)
        {    k=sq_read(&m1,bus_in_q2);
            if(k==0)
                      printf("BUS_IN_Q2 IS BEING DE-QUEUED\n");
            pthread_mutex_lock(&hold);
            rx_count++;
            printf("The no. of message received is : %d \n",rx_count);
            printf("====================================================================================\n");
            flag=0;
            pthread_mutex_unlock(&hold);
        }
   
        else;
   
        /*if(k==0)
            free(m1);*/
   
        next.tv_nsec=next.tv_nsec+period.tv_nsec;
        clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&next,NULL);

    }
sem_post(&s_act);


    pthread_exit(0);

    return NULL;

}




int main()
{

  pthread_t thread_id[6];//declaring sender threads//
  pthread_t r_thread;//declaring receiver threads//
  pthread_attr_t tattr[7];//declaring the attribute of the threads//
 
  int ret,i,j,k,r;

  struct param *temp;
  struct param *temp2;
 
  temp=malloc(sizeof(struct param));
  temp2=malloc(sizeof(struct param));
 
  bus_in_q1=sq_create();//creating shared queue-1//
  bus_in_q2=sq_create();//creating shared queue-2//
  
  const int p_period_multiplier[6]={12,32,25,18,28,19}; //multiplier constants to set the periodicty of the sender threads//
  const int r_period_multiplier[1]={17};//multiplier constant to set the periodicity of the reciver thread//
 
  int s_id[6]={0,1,2,3,4,5};//initializing the source_id//
  int r_id=0;//initilaizing the reciver_id//
 
  const int* sid[6]={&s_id[0],&s_id[1],&s_id[2],&s_id[3],&s_id[4],&s_id[5]};//initializing the constant integer pointer to the source_ids//
  const int * rid= &r_id;//initializing the constant integer pointer to the receiver_ids//

  const int new_prior[7]={90,91,92,93,94,95,96};

    struct sched_param param;
    int policy=SCHED_FIFO;
   
   
   


  for(i=0;i<6;i++)
  {
    temp->a =(int *)sid[i];//loading the source_id pointer to the struct//
        temp->b=p_period_multiplier[i];//loading the periodicty multiplier to the struct//
    pthread_attr_init(&tattr[i]);
    pthread_attr_getschedparam (&tattr[i],&param);
    pthread_attr_setschedpolicy(&tattr[i],policy);
    param.sched_priority=new_prior[i];
    pthread_attr_setschedparam(&tattr[i],&param);
        pthread_create(&thread_id[i],&tattr[i],sender_function,(void *)temp);//creating the sender thread//
       sleep(0.2);
  }
 
 
 
  temp2->a=(int *)rid;//loading the reciver_id pointer to the struct//
  temp2->b=r_period_multiplier[0]; //loading the periodicty multiplier to the struct//
 
  pthread_create(&r_thread,NULL,receiver_function,(void *)temp2);//creating reciever thread//
  sleep(0.2);
       

 


   for(j=0;j<6;j++)
      pthread_join(thread_id[j],NULL);

   pthread_join(r_thread,NULL);
 
   sq_delete(bus_in_q1);
   sq_delete(bus_in_q2);
 
   return 0;

}
