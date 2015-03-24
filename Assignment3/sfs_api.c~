
#include "sfs_api.h"
#include "disk_emu.c"


#define FILENAME "FILENAME" //filename
#define BLOCK_SIZE 512 //block size of 512 bytes
#define NUM_BLOCKS 100 // test, to be changed

int unusedBlocks[1000];
int mapDisk[1000][1000];

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
};

int printDiskMap(char array[1000][1000]){
    int i;
    int j;
    
    for(i = 0; i < NUM_BLOCKS; i++){
        printf("%c\n", array[i][0]);
    }
    
    return 0;
    
}

int mksfs(int fresh){
    int result;
    int i,j;
    char *output;
    output = malloc(1000);
    
    if(fresh){ //if it does not exist, create one fresh
        init_fresh_disk(FILENAME, BLOCK_SIZE, NUM_BLOCKS);
        
        
        //keeps track of unused blocks
        for(i = 0; i < NUM_BLOCKS; i++){
            unusedBlocks[i] = 0;
        }
        
        
    }else{ //if it already exists
        init_disk(FILENAME, BLOCK_SIZE, NUM_BLOCKS);
        
        //reproduce a 2D array with the information on the disk
        for(i = 0; i < NUM_BLOCKS; i++){
                mapDisk[i][0] = read_blocks(0, i, output);
        }
        
        printDiskMap(mapDisk);
        
        
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
    mksfs(1);
		return 0;
}
