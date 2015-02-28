
 
  //=============== CODE BELOW IS EXAMPLE OF MULTIPLE THREADS RUNNING A FUNCTION =========
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

static sem_t rw_mutex = 1; //common to both readers
static sem_t mutex = 1; //mutual exclusion when read_count is used
static int globalVariable = 0; //integer that thread must read
static int read_count = 0; // number of readers currently reading

void *functionC();
void *writer();
void *reader();
void *createWriterThreads();

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int  counter = 0; //thread counter


int main()
{
    pthread_t threadWriter[10], threadReader[100];
    sem_init(&mutex,0,1);
    sem_init(&rw_mutex,0,1);

    //create 10 writer threads
    for(int i = 0; i < 10; i++){
        pthread_create( &threadWriter[i], NULL, &writer, NULL);
    }
    //create 100 reader threads
    for(int i = 0; i < 100;i++) {
        pthread_create( &threadReader[i], NULL, &reader, NULL);
    }
    
    //join writer threads
    for(int i = 0; i < 10;i++){
        pthread_join(threadWriter[i], NULL);
    }
    //join reader threads
    for(int i = 0; i < 100;i++){
        pthread_join(threadReader[i], NULL);
    }
    
    
    
    
    exit(0);
}


void *reader(){
    
    while(TRUE){
        sleep(1); // to modify [0,100] miliseconds
        wait(&mutex);
        read_count++;
        if(read_count == 1){
            wait(&rw_mutex);
        }
        sem_post(&mutex); // signal(mutex)
        
        //read is performed
        printf("Reader : Global Variable = %i \n", globalVariable);
        
        wait(&mutex);
        read_count--;
        
        if(read_count == 0) {
            sem_post(&rw_mutex); //signal(mutex)
        }
        sem_post(&mutex); //signal(mutex)
    }
    return 0;
}

void *writer(){
    
    
    
    while(read_count == 0){
        sleep(1); //to modify [0,100] milliseconds
        wait(&rw_mutex);
        // write is performed
        globalVariable = globalVariable + 10;
        printf("Writer : Global Variable = %i \n", globalVariable);
        sem_post(&rw_mutex);
    }
    
    return 0;
}

