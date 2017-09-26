
#include "queue.h"

#define MOUSEFILE "/dev/input/event2"


#define CPU_CLOCK_SPEED 1700000  // MY CPU CLOCK SPEED IS 1.7 GHz //


       //DEFINE THE QUEUE LENGTH TO 10 //


#define BASE_PERIOD 1000    //MEASUREMENT IN MICRO SECONDS //


#define message_length 25     //DEFINING THE TOTAL MESSAGE LENGTH //



double sum=0,sum1=0;
int flag=0,flag1,flag2;
float queuing_time[5888888];
int globe=0;
int count=0; // TO COUNT THE TOTAL NUMBER OF MESSAGE BEING TRANSMITTED //
int rx_count=0;

     int f1;
     double std_deviation;
     double average;
struct param
{
    int *a;
    int b;
};




queue_t* bus_in_q1;
queue_t* bus_in_q2;

pthread_mutex_t hold=PTHREAD_MUTEX_INITIALIZER; //INITIALZING THE MUTEX  LOCK //




void *sender_function(void *ptr)
{
    	struct param new=*(struct param *)ptr;
   	 int source_id;
    	source_id=*(new.a); // FETCHING THE SOURCE_ID //
 
    	sem_t s_act; // INITIALIZE THE SEMAPHORE //
 
        double random,n; // INITIAIZE RANDOM VARIABLES //
 
        double i, j;     // NO. OF ITERATIONS AND  CONTROL VARIABLES //
        double f;        // FACTOR THAT REPEATS //
        double pi;      // THE PI-VALUE //
 


   	 printf("The passed multiplier is : %d \n",new.b); // RECEIVING THE PERIODICITY MULTIPLIER OF THE RESPECTIVE SENDER THREAD //
 
    	struct timespec next,period; // INITIALIZE THE VARIABLES OF STRUCTURE TIMESPESC //
    	sem_init(&s_act,0,1); // INITIALIZE THE SEMAPHORE //
    	sem_wait(&(s_act)); // INDICATES THE OTHER THREAD TO WAIT UNTIL THE COMPUTATION IS COMPLETED //
    	period.tv_nsec=new.b*BASE_PERIOD*1000; // SETTING THE PERIOD IN MICRO SECONDS //
 
    	clock_gettime(CLOCK_MONOTONIC,&next); // TO GET THE PRESENT TIME //

 
    	while(1)
    	{
       		if(globe==1)break;
  
         	else{ struct message *m1; // INITIALIZE THE MESSAGE STRUCTURE //
        		m1=(struct message *)malloc(sizeof(struct message));
 
        		m1->src_id=source_id; // ASSIGN THE SOURCE_ID //
        		printf("SOURCE_ID: %d\t",m1->src_id);
        		if(source_id==0 || source_id==1 ||source_id==2) // ENQUEUE TO THE DATA_QUEUE-1 //
       			 {
            			m1->destn_id=0;
            			printf("DESTINATION_ID: %d\t",m1->destn_id);
       			 }
 
 
        		if(source_id==3 || source_id==4 ||source_id==5) // ENQUEUE TO THE DATA_QUEUE-2 //
        		{
           		 m1->destn_id=1;
            		printf("DESTINATION_ID: %d\t",m1->destn_id);
        		}
  
        		pthread_mutex_lock (&hold); // LOCK THE GLOBAL VARIABLE //
              		 m1->msg_id=++count; // INCREMENT THE COUNT //
        		printf("MSG_ID: %d\t\n",count);
        		pthread_mutex_unlock (&hold); // UNLOCK THE GLOBAL VARIABLE //
 

    		pi = 1;
    		random=rand()%51; // GENERATE A RANDOM NUMBER BETWEEN 1 AND 50 //
 
    		if(random<=10)    // TO ENSURE THAT THE GENERATED RANDOM NUMBER IS BETWEEN 11 AND 50 //
        	{
        		random+=11;
       		}
       		else;
 
       // Approximation of the number PI through the Viete's series //
 
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
        {    sq_write(m1,bus_in_q1);
             printf("===================================================================\n");
        }
    if(m1->destn_id==1)
        {    sq_write(m1,bus_in_q2);
                 printf("===================================================================\n");
        }
}
   if((next.tv_nsec+period.tv_sec)>1000000000)
    {   next.tv_nsec=((next.tv_nsec+period.tv_nsec)%1000000000);
        next.tv_sec++;
    }
 else    next.tv_nsec=next.tv_nsec+period.tv_nsec;
    clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&next,NULL);
 


    }
    sem_post(&(s_act));
  
    pthread_exit(0);
 
    return NULL;

}


void *sender_function1(void *ptr)
{    printf("I have reached sender function 1\n");
    
 
    sem_t s_act; // INITIALIZE THE SEMAPHORE //
 
        double random,n; // INITIAIZE RANDOM VARIABLES //
  
        double i, j;     // NO. OF ITERATIONS AND  CONTROL VARIABLES //
        double f;        // FACTOR THAT REPEATS //
        double pi;      // THE PI-VALUE //

    int fd,c,d,cr,dr;
       float l,r,mn,lr,rr,mnr;
        float la[2],lra[2];
    struct input_event ie;


    if((fd=open(MOUSEFILE,O_RDONLY))==-1)
    {
            perror("opening device");
            exit(EXIT_FAILURE);
    }
 
    c=0;cr=0;d=0;dr=0;la[0]=0;la[1]=0;lra[0]=0;lra[1]=0;
 while((read(fd,&ie,sizeof(struct input_event))))

{   
    if(ie.code==272)
    {
                c++;
                if(c%2==0)
               {d=c/2;
                l=ie.time.tv_sec%100;
                r=ie.time.tv_usec/1000;
                l=l+r*0.001;
               d=d%2;

               
                              
                if(d==0)la[d]=l;
                if(d==1) la[d]=l;
                mn=(la[0]-la[1]);
printf("left click  %f  c= %d  d=%d\n",l,c/2,d);
                   
if(mn<0.50 && mn >-0.5)
                {printf("Double click detected\n");pthread_mutex_lock(&(hold));globe=1;pthread_mutex_unlock(&(hold)); return 0;}
else{
struct message *m1; // INITIALIZE THE MESSAGE STRUCTURE //
            m1=(struct message *)malloc(sizeof(struct message));
 
        m1->src_id=2; // ASSIGN THE SOURCE_ID //
        printf("SOURCE_ID: %d\t",m1->src_id);
        // ENQUEUE TO THE DATA_QUEUE-1 //
        {
            m1->destn_id=0;
            printf("DESTINATION_ID: %d\t",m1->destn_id);
       
              
      
      
 
 
 
        pthread_mutex_lock (&hold); // LOCK THE GLOBAL VARIABLE //
               m1->msg_id=++count; // INCREMENT THE COUNT //
        printf("MSG_ID: %d\t\n",count);
        pthread_mutex_unlock (&hold); // UNLOCK THE GLOBAL VARIABLE //
 

    pi = 1;
    random=rand()%51; // GENERATE A RANDOM NUMBER BETWEEN 1 AND 50 //
 
    if(random<=10)    // TO ENSURE THAT THE GENERATED RANDOM NUMBER IS BETWEEN 11 AND 50 //
        {
        random+=11;
       }
       else;
 
       // Approximation of the number PI through the Viete's series //
 
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

         sq_write(m1,bus_in_q1);
             printf("===================================================================\n");
      
  }

       }
    }}

//==============================================================================================================//
            

     if(ie.code==273)
    {

        cr++;
                if(cr%2==0)
                {dr=cr/2;
                lr=ie.time.tv_sec%100;
                rr=ie.time.tv_usec/1000;
                lr=lr+rr*0.001;
            dr=dr%2;
               
                      
                if(dr==0)lra[dr]=lr;
                if(dr==1) lra[dr]=lr;
                mnr=(lra[0]-lra[1]);
      
        if(mnr<0.50 && mnr>-0.5);
     else
        {
printf("right click  %f  c= %d  d=%d\n",lr,cr/2,dr);
struct message *m1; // INITIALIZE THE MESSAGE STRUCTURE //
        m1=(struct message *)malloc(sizeof(struct message));
 
        m1->src_id=5; // ASSIGN THE SOURCE_ID //
        printf("SOURCE_ID: %d\t",m1->src_id);
       
 
        // ENQUEUE TO THE DATA_QUEUE-2 //
        {
            m1->destn_id=1;
            printf("DESTINATION_ID: %d\t",m1->destn_id);
        }
  
        pthread_mutex_lock (&hold); // LOCK THE GLOBAL VARIABLE //
               m1->msg_id=++count; // INCREMENT THE COUNT //
        printf("MSG_ID: %d\t\n",count);
        pthread_mutex_unlock (&hold); // UNLOCK THE GLOBAL VARIABLE //
 

    pi = 1;
    random=rand()%51; // GENERATE A RANDOM NUMBER BETWEEN 1 AND 50 //
 
    if(random<=10)    // TO ENSURE THAT THE GENERATED RANDOM NUMBER IS BETWEEN 11 AND 50 //
        {
        random+=11;
       }
       else;
 
       // Approximation of the number PI through the Viete's series //
 
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

  
        {    sq_write(m1,bus_in_q2);
                 printf("===================================================================\n");
        }
}
   
 



}




}}
    sem_post(&(s_act));
  
    pthread_exit(0);
 
    return NULL;
}


void *receiver_function(void *ptr)

{
    struct param new=*(struct param *)ptr;
    sem_t s_act;
    int receiver_id;
    int k;
    struct timespec next,period;
   period.tv_nsec=new.b*BASE_PERIOD*1000;
   clock_gettime(CLOCK_MONOTONIC,&next);
  

    sem_init(&s_act,0,1);
    sem_wait(&s_act);
   
    while(1)
    {    

                if(globe==1)break;
            else {
                    struct message *m1=malloc(sizeof(struct message));
                    receiver_id=*(new.a);
                    receiver_id=receiver_id+(rand()%2);
                    printf("\n======================================================\n");
                    printf("The received multiplier is : %d\n",new.b);
                    if(flag==0)
                    {    k=sq_read(&m1,bus_in_q1);
                            if(k==0)
                               { printf("BUS_IN_Q1 IS BEING DE-QUEUED\n");
                             pthread_mutex_lock(&hold);
                                rx_count++;
                           pthread_mutex_unlock(&hold);           
                           queuing_time[rx_count-1]=((double)(m1->tsc1))/CPU_CLOCK_SPEED;
                             printf("The no. of message received is : %d \n",rx_count);
                        flag=1;queuing_time[rx_count]='\0';
                        printf("THe accumulated queueing time of msg:%d is %f MILLI SECONDS\n",m1->msg_id,queuing_time[rx_count-1]);
                         }
       
                           if(k==-1)printf("OPERAION FAILED\n");
   
                    }
 
                    else if(flag==1)
                    {    k=sq_read(&m1,bus_in_q2);
                            if(k==0)
                                  {printf("BUS_IN_Q2 IS BEING DE-QUEUED\n");
                                pthread_mutex_lock(&hold);
                            rx_count++;
                        pthread_mutex_unlock(&hold);          
                        queuing_time[rx_count-1]=((double)(m1->tsc1))/CPU_CLOCK_SPEED;
                                printf("The no. of message received is : %d \n",rx_count);
                            queuing_time[rx_count]='\0';
                        printf("THe accumulated queueing time of msg:%d is %f MILLI SECONDS\n",m1->msg_id,queuing_time[rx_count-1]);
                            flag=0;}
                     

                           if(k==-1)printf("OPERAION FAILED\n");
  
      
                    }
                      
  
            printf("====================================================================================\n");
 
                    }
        
 if((next.tv_nsec+period.tv_nsec)>1000000000)
    {   next.tv_nsec=((next.tv_nsec+period.tv_nsec)%1000000000);
        next.tv_sec++;
    }

    else  next.tv_nsec=next.tv_nsec+period.tv_nsec;
      
        clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&next,NULL);

    }

sem_post(&s_act);

pthread_exit(0);

    return NULL;

}






void *receiver_function1(void *ptr)

{
     int k1,k2;
     sem_t s_act;
     sem_init(&s_act,0,1);


     printf("Reached receiver11111 function\n");    
   
sem_wait(&s_act);
    while(1)
{    if(globe==1)      
      {    printf("Double click detected...oops\n");
             while(flag1!=2 || flag2!=2)
            {
                struct message *m1=malloc(sizeof(struct message));
   
                printf("\n======================================================\n");
       
                if(flag==0)
                {    k1=sq_read(&m1,bus_in_q1);
                     if(k1==-1){printf("OPERAION FAILED\n"); flag1=2;break;}        
                 else if(k1==0)
                           { printf("BUS_IN_Q1 IS BEING DE-QUEUED\n");
                             pthread_mutex_lock(&hold);
                                rx_count++;
                             queuing_time[rx_count-1]=((double)(m1->tsc1))/CPU_CLOCK_SPEED;
                        queuing_time[rx_count]='\0';
                        printf("The accumulated queueing time of msg:%d is %f MILLI SECONDS\n",m1->msg_id,queuing_time[rx_count-1]);
                             printf("The no. of message received is : %d \n",rx_count);
                                  pthread_mutex_unlock(&hold);free(m1);
                             flag=1;}
        
                       else;

                }
 
                else if(flag==1)
                {    k2=sq_read(&m1,bus_in_q2);
                         if(k2==-1){printf("OPERAION FAILED\n"); flag2=2;break;}
                    else if(k2==0)
                                {  printf("BUS_IN_Q2 IS BEING DE-QUEUED\n");
                        pthread_mutex_lock(&hold);
                            rx_count++;
                            queuing_time[rx_count-1]=((double)(m1->tsc1))/CPU_CLOCK_SPEED;
                queuing_time[rx_count]='\0';
                    printf("The accumulated queueing time of msg:%d is %f MILLI SECONDS\n",m1->msg_id,queuing_time[rx_count-1]);
                            printf("The no. of message received is : %d \n",rx_count);
                      pthread_mutex_unlock(&hold);free(m1);
                            flag=0;}
                         else;         
       
 
                }
            else;

   
 for(f1=0;f1<rx_count;f1++)
       {
                    sum=sum+queuing_time[f1];
            }
            average=(double)sum/rx_count;

            for(f1=0;f1<rx_count;f1++)
            {
                    sum1=sum1+pow((queuing_time[f1]-average),2);
            }
            std_deviation=(double)(sqrt(sum1/rx_count));
    }
break;    
}
    else;
      
}
sem_post(&s_act);


  
      
 
      
       
 
       
 




    pthread_exit(0);

    return NULL;

}



int main()
{

      pthread_t thread_id[4];// declaring sender threads //
      pthread_t apthread_id[2];// declraing aperiodic thread //
      pthread_t r_thread;// declaring receiver threads //
      pthread_attr_t ptattr[4];// declaring the attributes of the periodic thread //
      pthread_attr_t atattr[2];// declaring the attributes of the aperiodic thread //
    pthread_attr_t rtattr;
int i,j;

  



  struct param *temp;
  struct param *temp2;
temp=malloc(sizeof(struct param));
temp2=malloc(sizeof(struct param));

bus_in_q1=sq_create();//creating shared queue-1//
      bus_in_q2=sq_create();

const int p_period_multiplier[4]={12,32,18,28};
const int r_period_multiplier[1]={40};

int sp_id[6]={0,1,3,4};
 int sa_id[2]={2,5};
int r_id=0;
const int* sid[6]={&sp_id[0],&sp_id[1],&sp_id[2],&sp_id[3],&sa_id[0],&sa_id[1]};
const int * rid= &r_id;

const int new_pprior[4]={91,93,92,95};
 const int new_aprior[2]={97,98};
const int new_rprior[1]={90};

struct sched_param param;
int policy=SCHED_FIFO;

for(i=0;i<4;i++)
      {
            temp->a =(int *)sid[i];//loading the source_id pointer to the struct//
            temp->b=p_period_multiplier[i];//loading the periodicty multiplier to the struct//
            pthread_attr_init(&ptattr[i]);
           pthread_attr_getschedparam (&ptattr[i],&param);
            pthread_attr_setschedpolicy(&ptattr[i],policy);
            param.sched_priority=new_pprior[i];
            pthread_attr_setschedparam(&ptattr[i],&param);
               pthread_create(&thread_id[i],&ptattr[i],sender_function,(void *)temp);//creating the sender thread//
               sleep(0.2);
        }



    

for(i=0;i<2;i++)
        {pthread_attr_init(&atattr[0]);
            pthread_attr_getschedparam (&atattr[0],&param);
            pthread_attr_setschedpolicy(&atattr[0],policy);
            param.sched_priority=new_aprior[0];
            pthread_attr_setschedparam(&atattr[0],&param);
            pthread_create(&apthread_id[0],&atattr[0],sender_function1,NULL);//creating the sender thread//
               sleep(0.2);}


pthread_attr_init(&rtattr);
      pthread_attr_getschedparam (&rtattr,&param);
      param.sched_priority=new_rprior[0];
     pthread_attr_setschedparam(&rtattr,&param);
      temp2->a=(int *)rid;//loading the reciver_id pointer to the struct//
      temp2->b=r_period_multiplier[0]; //loading the periodicty multiplier to the struct//
     pthread_create(&r_thread,&rtattr,receiver_function,(void *)temp2);//creating reciever thread//
      sleep(0.2);
 
     
       pthread_attr_init(&rtattr);
      pthread_attr_getschedparam (&rtattr,&param);
      param.sched_priority=new_rprior[0];
     pthread_attr_setschedparam(&rtattr,&param);
      temp2->a=(int *)rid;//loading the reciver_id pointer to the struct//
      temp2->b=r_period_multiplier[0]; //loading the periodicty multiplier to the struct//
     pthread_create(&r_thread,&rtattr,receiver_function1,(void *)temp2);//creating reciever thread//
      sleep(0.2);
 
for(j=0;j<4;j++)
              pthread_join(thread_id[j],NULL);

  for(j=0;j<2;j++)
                pthread_join(apthread_id[j],NULL);

 pthread_join(r_thread,NULL);


 printf("====================================================================================\n");
 

            printf("Therefore the total number of messages received at the reciver thread is :%d...,the numbr of messages droped is %d...,the average is : %lf MILLI SECONDS...and the standard deviation is : %lf MILLI SECONDS\n ",rx_count,(count-rx_count),average,std_deviation);



sq_delete(bus_in_q1);
sq_delete(bus_in_q2);


 
return 0;

}
