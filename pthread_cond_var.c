/*Dinning p_thread_id using Monitor*/
#include<stdio.h>
#include<pthread.h>
#include<string.h>

extern int random_utime(void);

pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;

#define NUM_PHILOSOPHER  5

pthread_cond_t status; /* cv */

int activity[NUM_PHILOSOPHER];//0-think  1- hungry  2- eat
int activity_counter_eat[NUM_PHILOSOPHER];

#define TRACE(msg,val)  printf("PHILOSOPHER %d IS %s\n",val,msg) //("%s :%d \n", msg,val)

enum {
	THINK,
	HUNGRY,
	EAT
} state_t;

void pick_fork(int* id)
{
  pthread_mutex_lock(&mutex);

  activity[*id] = HUNGRY;
  TRACE("HUNGRY",*id);
  if(activity[(*id+1)%NUM_PHILOSOPHER]!= EAT && activity[(*id+NUM_PHILOSOPHER-1)%NUM_PHILOSOPHER]!=EAT) {
  	activity [*id]=EAT;
//   	TRACE("EATING",*id);
        TRACE("PICKING LEFT FORK",*id);
        TRACE("PICKING RIGHT FORK",*id);
        TRACE("EATING",*id);
 
       activity_counter_eat[*id]++;
  }
  else {
   	TRACE("WAITING",*id);
  	pthread_cond_wait(&status,&mutex);
  }
  pthread_mutex_unlock(&mutex);
}

void put_fork(int* id)
{
  pthread_mutex_lock(&mutex);

  activity[*id]=THINK;
    TRACE("THINKING",*id);
  pthread_cond_signal(&status);
 
  pthread_mutex_unlock(&mutex);
}

void* dine(void* id)
{
    int* i;
    i =(int*) id;

  //  printf("Philospher %d sits at table\n",*i);
    TRACE("SITTING AT TABLE, SAYS HELLO!!!",*i);
 

	while(1)
	{
//		TRACE("PHILOSPHER", *i);	
		do {
   		       if(activity_counter_eat[*i]==5)
                              goto Exit;

			pick_fork(i);
		} while (activity[*i] == HUNGRY);
                 
   		 put_fork(i);
		 usleep(random_utime());               
	}
    Exit:  
    TRACE("LEAVING THE TABLE,SAYS BYE!!!",*i);
    pthread_exit(NULL);
}
/*
void* show_counter(void* arg)
{
 	 int i=0;
	 while(1){
 		for( i=0;i<NUM_PHILOSOPHER;i++){
			printf("%d\t", activity_counter_eat[i]);
		}  
		printf("\n");
  	}
    pthread_exit(NULL);
}

*/

int main ()
{
  int rc =0;
  int j =0;
  int ret_val=0;
  int arg=0;

  pthread_t p_thread_id[NUM_PHILOSOPHER];
	int eat_stats_id[NUM_PHILOSOPHER];
  	int 	    p_id[NUM_PHILOSOPHER];

//  pthread_t p_thread_observer;

  for(j=0;j< NUM_PHILOSOPHER;j++) {
  	p_id[j]=j;
  	rc = pthread_create(&p_thread_id[j], NULL, &dine, &p_id[j]);
   	if(rc!=0)
    		 printf("Error Thread Creation");
  }

  /* ret_val=pthread_create(&p_thread_observer,NULL,&show_counter,&arg);
   if(ret_val!=0)
                printf("Observer Thread creation error");
 */

   for(j=0;j<NUM_PHILOSOPHER;j++) {
		  pthread_join(p_thread_id[j],NULL); 
  	}
    
//    pthread_join(p_thread_observer,NULL);

  pthread_exit(NULL);
  return (0);
}
