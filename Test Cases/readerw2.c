#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>


int readcount=0;
int writecount=0;
sem_t x=1,y=1,z=1,wsem=1,rsem=1;


void *writer(void *i);
void *reader(void *i);

int main()
{
  int i,res;
  void *thread_result;
  pthread_t rdr[3],wtr[2];
  sem_init(&x,0,1);
  sem_init(&wsem,0,1);
  sem_init(&y,0,1);
  sem_init(&rsem,0,1);
  sem_init(&z,0,1);
  while(1)
  {
  for(i=0;i<3;i++)
  {
    res=pthread_create(&rdr[i],NULL,reader,(void *)i);
    if(res!=0)
    {
       perror("\nthread creation failed");
       exit(EXIT_FAILURE);
    }
   // printf("\nThread %d for reader created",i);
  }
  for(i=0;i<2;i++)
 {
   res=pthread_create(&wtr[i],NULL,writer,(void *)i);
    if(res!=0)
    {
       perror("\nthread creation failed");
       exit(EXIT_FAILURE);
    }
    //printf("\nThread %d for writer created",i);
  }
  for(i=0;i<3;i++)
  {
     res=pthread_join(rdr[i],&thread_result);
      if(res!=0)
      {
        perror("Thread joining fails");
        exit(EXIT_FAILURE);
       }
  }
  for(i=0;i<2;i++)
  {
    
     res=pthread_join(wtr[i],&thread_result);
      if(res!=0)
      {
        perror("Thread joining fails");
        exit(EXIT_FAILURE);
       }
  }
  }
 exit(EXIT_SUCCESS);
}

void *reader(void *i)
{
   int v;
   v=(int )i;
   while(1)
   {
     printf("\nreader %d wants to read",v);
     sem_wait(&z);
     sem_wait(&rsem);
     sem_wait(&x);
     readcount++;
     if(readcount==1)
       {
        sem_wait(&wsem);
	}
        sem_post(&x);
        sem_post(&rsem);
        printf("\nreader %d  is reading",v);
        sleep(rand()%8);
        printf("\nreader %d finished reading ",v); 
        sem_wait(&x);
        readcount--;
        if(readcount==0)
           sem_post(&wsem);
        sem_post(&x);
   break;
  }
}


void *writer(void *i)
{
  int v;
  v=(int )i;
  while(1)
  {
    printf("\nWriter %d wants to write",v);
    sem_wait(&y);
    writecount++;
    if(writecount==1)
     {
       sem_wait(&rsem);
     }
    sem_post(&y);
    sem_wait(wsem);
     printf("\nwriter %d  is writing",v);
     sleep(rand()%8);
     printf("\nwriter %d finished writing ",v); 
     sem_post(wsem);
     sem_wait(&y);
     writecount--;
     if(writecount==0)
      sem_post(&rsem);
     sem_post(&y);
     break;
  }
}