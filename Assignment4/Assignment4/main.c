//
//  main.c
//  Assignment4
//
//  Created by Tuan Kiet Luu on 2015-04-09.
//  Copyright (c) 2015 Tuan Kiet Luu. All rights reserved.
//

#include <unistd.h>
#include <stdio.h>

//Initialize policies
int FIRST_FIT = 10;
int BEST_FIT = 20;
int currentPolicy = 10; //start with first fit as current policy
char *my_malloc_error = "Malloc : Success"; // error condition
int MAX_MEMORY = 10240000; //128Kb

struct block{
    //int isAllocated; //allocated = 1, free = 0
    int length; // remaining bytes of free block
    struct block *prevBlock; //points to the previous free block
    struct block *nextBlock; //points to the next free block
    void *freeBlock; //points to the start of the free block
}block;

int BLOCK_SIZE = 8*sizeof(block);
struct block *freeListHead;

void *findFreeBlock(int size){ //finds the next free block of a given size
    printf("freeListHead : nextBlock %td\n", freeListHead->nextBlock);


    
    if(freeListHead == NULL){
        return NULL; //there is no free list
    }
    
    //printf("in findFreeBlock");
    struct block *search = freeListHead;
    
    //FIRST_FIT algorithm
    
    if(currentPolicy == FIRST_FIT){ /* works */
        
        printf("in FIRST_FIT policy\n");
        //while the next block is not the end
        while(search != NULL){
            printf("in while loop \n");
            if(search->length >= size){ //if the block has enough space, return it
                
                printf("search : starts at %td\n", search);
                printf("search : free data starts at %td\n", search->freeBlock);
                printf("search : length is %d\n", search->length);
                printf("search : Size of a block is = %lu\n", 8*sizeof(block));
                return search;

            }else{ //else, search in the next block
                search = search->nextBlock;


            }
        }
        return NULL;
    }else{ //BEST_FIT algorithm
        
        struct block *bestFitBlock = freeListHead;
        bestFitBlock->length = 10*size; //initialize high temporary smallest for BEST_FIT

        //loop through all the list
        if(search->nextBlock != NULL){
            if(search->length < bestFitBlock->length && search->length >= size){
                //found a better fit (smallest)
                bestFitBlock = search;
                
            }
        }

        printf("bestFitBlock : starts at %td\n", bestFitBlock);
        printf("bestFitBlock : free data starts at %td\n", bestFitBlock->freeBlock);
        printf("bestFitBlock : length is %d\n", bestFitBlock->length);
        printf("bestFitBlock : Size of a block is = %lu\n", 8*sizeof(block));
        return bestFitBlock;
        
    }
        
}



/*
 REMINDER : 
 brk() and sbrk() change the location of the program break, which
 defines the end of the process's data segment (i.e., the program
 break is the first location after the end of the uninitialized data
 segment).
 */
void *my_malloc(int size){
//    printf("===== Testing my_alloc() =====\n");
//    printf("sbrk(0) = %p \n", sbrk(0));
    int sizeInBit = 8*size;
    
    //initlializes initial heap
    if(freeListHead == NULL){
        printf("freeListHead == NULL : Initializing freeListHead...\n");
        freeListHead = sbrk(0); //initialize the free list head
        brk(sbrk(0) + sizeof(block) + MAX_MEMORY); //set PB to location

        freeListHead->length = MAX_MEMORY; //128kb
        freeListHead->prevBlock = NULL;
        freeListHead->nextBlock = NULL;
        freeListHead->freeBlock = (void *)freeListHead + BLOCK_SIZE;
        
        /* works */
        printf("freeListHead : Size of a block is = %lu\n", BLOCK_SIZE);
        printf("freeListHead : starts at %td\n", freeListHead);
        printf("freeListHead : freeBlock = %td\n", freeListHead->freeBlock); //tested : difference = 256
        printf("freeListHead : length is %d\n", freeListHead->length);


        
    }
    
    struct block *mallocFreeBlock = findFreeBlock(sizeInBit + BLOCK_SIZE); //returns pointer that has a free block
    
    /* works */
    printf("mallocFreeBlock : starts at %td\n", mallocFreeBlock);
    printf("mallocFreeBlock : freeBlock = %td\n", mallocFreeBlock->freeBlock); //tested : difference = 256
    printf("mallocFreeBlock : length is %d\n", mallocFreeBlock->length);
    
    
    //if size is too large, therefore no free blocks big enough
    if(mallocFreeBlock == NULL){
        my_malloc_error = "Malloc : ERROR (size too big, could not find free space)";
        printf("%s\n", my_malloc_error);
        return NULL;
    }else{
        
        /* ERROR BELOW */
        
        struct block *dataBlock;
        struct block *newFreeListHead;
        
        newFreeListHead->length = 0;
        newFreeListHead->prevBlock = NULL;
        newFreeListHead->nextBlock = NULL;
        newFreeListHead->freeBlock = NULL;
        
        
        
        if(mallocFreeBlock == freeListHead){
            freeListHead = (freeListHead->freeBlock + sizeInBit);
            newFreeListHead = freeListHead;
        }else{
            newFreeListHead = mallocFreeBlock->freeBlock + sizeInBit;
        }
        
        printf("newFreeListHead1 : starts at %td\n", newFreeListHead);
        //printf("newFreeListHead1 : freeBlock = %td\n", newFreeListHead->freeBlock);
        printf("newFreeListHead1 : length is %d\n", newFreeListHead->length);

        //initializes the updated freelisthead

        newFreeListHead->length = mallocFreeBlock->length - sizeof(block) - size;
        newFreeListHead->prevBlock = mallocFreeBlock; //increments the free block with the size (size is now used)
        newFreeListHead->nextBlock = mallocFreeBlock->nextBlock;
        newFreeListHead->freeBlock = (void *)newFreeListHead + BLOCK_SIZE;
        
        /* works */
        printf("newFreeListHead2 : starts at %td\n", newFreeListHead);
        printf("newFreeListHead2 : freeBlock = %td\n", newFreeListHead->freeBlock); //the difference is length - sizeInBits (10*8) - 256 (BLOCK_SIZE)
        printf("newFreeListHead2 : length is %d\n", newFreeListHead->length);
        
        
        dataBlock = mallocFreeBlock;
        dataBlock->length = sizeInBit;
        dataBlock->nextBlock = newFreeListHead;
        
        
        
        printf("newFreeListHead : = %p\n", newFreeListHead);
        printf("newFreeListHead : freeBlock = %p\n", newFreeListHead->freeBlock);
        printf("newFreeListHead : length = %d\n", newFreeListHead->length);
        printf("newFreeListHead : Size of a block is = %lu\n", 8*sizeof(block));
        
        printf("dataBlock : = %p\n", dataBlock);
        printf("dataBlock : freeBlock =  %p\n", dataBlock->freeBlock);
        printf("dataBlock : length = %d\n", dataBlock->length);
        printf("dataBlock : Size of a block is = %lu\n", 8*sizeof(block));
//
        

    }
    

    
    return 0;
}

void my_mallopt(int policy){

    if(policy != BEST_FIT && policy != FIRST_FIT){
        printf("error in policy input\n");
        return;
    }
    
    currentPolicy = policy;
    printf("Current Policy is now %i \n", currentPolicy);
}



int main() {
    
    /* testing my_mallopt */
    printf("====== Testing my allopt ====== \n");
    //my_mallopt(20);
    void* data1 = my_malloc(10);
    void* data2 = my_malloc(100);

    //my_malloc(10);
    //my_malloc(200);
    
    printf("%s\n", my_malloc_error);

    return 0;
}
