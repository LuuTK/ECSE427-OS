
#include "sfs_api.h"
#include "disk_emu.c"


#define FILENAME "FILENAME" //filename
#define BLOCK_SIZE 512 //block size of 512 bytes
#define NUM_BLOCKS 100 // test, to be changed


struct super_Block
{
    char magic[10];
    int blockSize;
    float FileSystemSize;
    int iNodeTableLength;
    int rootDirectoryNumber;
}superBlock = { "0xAABB0005",BLOCK_SIZE,NUM_BLOCKS, NUM_BLOCKS,0};

struct i_node
{
    char mode[100];
    int linkCount;
    int uid;
    int gid;
    int size;
    int pointers[13];
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
    int startAddr = 0;
    int nblocks = 1;

/*
    char *test;
		test = (char *) malloc(15); 
		strcpy(test,"tutorialspoint");

		printf("%s\n", test); 
*/
		int i;
		char *test = "Hello";
        char *writeArray;
        void *ptr;
		char *data;
		int readResult;
		char *output;
		output = malloc(4000);


    init_disk(FILENAME, BLOCK_SIZE, NUM_BLOCKS);
    write_blocks(startAddr, nblocks, test);

   	read_blocks(startAddr, nblocks, output);

    
		for(i = 0; i<10;i++){
			printf("output = %s\n", ((char *)output));	

		}

		//free(test);
		//free(writeTo);
		//free(writeArray);
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


int main()
{
    printf("Hello World\n");
    //mksfs(1);
    sfs_fopen(FILENAME);
		return 0;
}
