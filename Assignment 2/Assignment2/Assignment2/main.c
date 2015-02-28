//
//  main.c
//  Assignment2
//
//  Created by Tuan Kiet Luu on 2015-02-25.
//  Copyright (c) 2015 Tuan Kiet Luu. All rights reserved.
//

/*
#include <stdio.h>

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}

*/



 
  //=============== CODE BELOW IS EXAMPLE OF MULTIPLE THREADS RUNNING A FUNCTION =========
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

static sem_t rw_mutex = 1; //common to both readers
static sem_t mutex = 1; //mutual exclusion when read_count is used
static int globalVariable = 0;
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
    
    /*
    // Create independent threads each of which will execute functionC
    
    if( (rc1=pthread_create( &thread1, NULL, &writer, NULL)) )
    {
        printf("Thread creation failed: %d\n", rc1);
    }
    
    if( (rc2=pthread_create( &thread2, NULL, &functionC, NULL)) )
    {
        printf("Thread creation failed: %d\n", rc2);
    }
    
    if( (rc3=pthread_create( &thread3, NULL, &functionC, NULL)) )
    {
        printf("Thread creation failed: %d\n", rc3);
    }
    
    // Wait till threads are complete before main continues. Unless we
    // wait we run the risk of executing an exit which will terminate
    // the process and all threads before the threads have completed.
    */
    
    for(int i = 0; i < 10; i++){
        pthread_create( &threadWriter[i], NULL, &writer, NULL);
    }
    for(int i = 0; i < 100;i++) {
        pthread_create( &threadReader[i], NULL, &reader, NULL);
    }
    //pthread_join( thread1, NULL);
    //pthread_join( thread2, NULL);
    for(int i = 0; i < 10;i++){
        pthread_join(threadWriter[i], NULL);
    }
    for(int i = 0; i < 100;i++){
        pthread_join(threadReader[i], NULL);
    }
    
    
    
    
    exit(0);
}

void *functionC()
{
    //pthread_mutex_lock( &mutex1 );
    counter++;
    printf("Counter value: %d\n",counter);
    //pthread_mutex_unlock( &mutex1 );
    return 0;
}

void *reader(){
    do{
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
    }while(TRUE);
    return 0;
}

void *writer(){
    
    
    
    do{
        sleep(1); //to modify [0,100] milliseconds
        wait(&rw_mutex);
        // write is performed
        globalVariable = globalVariable + 10;
        printf("Writer : Global Variable = %i \n", globalVariable);
        sem_post(&rw_mutex);
    }while(read_count == 0);
    
    return 0;
}


/*


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_message_function( void *ptr );

int main()
{
    pthread_t thread1, thread2;
    char *message1 = "Thread 1 message";
    char *message2 = "Thread 2 message";
    int  iret1, iret2;
    
    // Create independent threads each of which will execute function
    
    iret1 = pthread_create( &thread1, NULL, &print_message_function, (void*) message1);
    iret2 = pthread_create( &thread2, NULL, &print_message_function, (void*) message2);
    
    // Wait till threads are complete before main continues. Unless we
    // wait we run the risk of executing an exit which will terminate
    // the process and all threads before the threads have completed.
    
    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL);
    
    printf("Thread 1 returns: %d\n",iret1);
    printf("Thread 2 returns: %d\n",iret2);
    exit(0);
}

void *print_message_function( void *ptr )
{
    char *message;
    int counter;
    counter++;
    message = (char *) ptr;
    printf("%s \n", message);
    return 0;
}

*/