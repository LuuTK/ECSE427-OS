
#include "sfs_api.h"
#include "disk_emu.c"


#define FILENAME "FILENAME" //filename
#define BLOCK_SIZE 512 //block size of 512 bytes
#define NUM_BLOCKS 100 // test, to be changed

FILE* fp = NULL;


main()
{
    printf("Hello World\n");
    //mksfs(1);
    sfs_fread(FILENAME, );
}


int mksfs(int fresh){
    int result;
    
    if(fresh){
        init_fresh_disk(FILENAME, BLOCK_SIZE, NUM_BLOCKS);
    }else{
        printf("failure");
    }
    
    
    return 0; // -1 if failed
}
int sfs_fopen(char *name){
    int i = 0;
    int j = 0;
    fp = fopen (FILENAME, "w+b");

    if (fp == NULL)
    {
        printf("Could not create new disk file %s\n\n", filename);
        return -1;
    }
    
    /*Fills the file with 0's to its given size*/
    for (i = 0; i < NUM_BLOCKS; i++)
    {
        for (j = 0; j < BLOCK_SIZE; j++)
        {
            fputc(0, fp);
        }
    }
    
    return 0;
}
int sfs_fclose(int fileID){
    return 0;
}
int sfs_fwrite(int fileID, const char *buf, int length){
    return 0;
}
int sfs_fread(int fileID, char *buf, int length){

    return 0;
}
int sfs_fseek(int fileID, int offset){
    return 0;
}
int sfs_remove(char *file){
    return 0;
}
int sfs_get_next_filename(char* filename){
    return 0;
}
int sfs_GetFileSize(const char* path){
    return 0;
}

