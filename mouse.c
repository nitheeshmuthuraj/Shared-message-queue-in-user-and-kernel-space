#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<linux/input.h>
#include<unistd.h>
#define MOUSEFILE "/dev/input/event2"

int main()
{
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

       if(ie.code==272){
            c++;
                if(c%2==0)
                {d=c/2;
                l=ie.time.tv_sec%100;
                r=ie.time.tv_usec/1000;
                l=l+r*0.001;  
        d=d%2;
                printf("left click  %f  c= %d  d=%d\n",l,c/2,d);
                          
                if(d==0)la[d]=l;
                if(d==1) la[d]=l;
                mn=(la[0]-la[1]);
                if(mn<0.50 && mn >-0.5)
        {printf("%f",mn); return 0;
              }
                }
            }
  
               if(ie.code==273){
            cr++;
                if(cr%2==0)
                {dr=cr/2;
                lr=ie.time.tv_sec%100;
                rr=ie.time.tv_usec/1000;
                lr=lr+rr*0.001;  
        dr=dr%2;
                printf("right click  %f  c= %d  d=%d\n",lr,cr/2,dr);
                          
                if(dr==0)lra[dr]=lr;
                if(dr==1) lra[dr]=lr;
                mnr=(lra[0]-lra[1]);
                if(mnr<0.50 && mnr>-0.5)
        {printf("%f",mnr); return 0;
              }
                }
            }
      
     
}
return 0;
}
