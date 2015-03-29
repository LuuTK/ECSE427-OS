#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sfs_api.h"
#include "disk_emu.h"
//#include "disk_emu.c"


#define FILENAME "FILENAME" //filename
#define BLOCK_SIZE 512 //block size of 512 bytes
#define NUM_BLOCKS 100 // test, to be changed
#define NUM_INODES 100
#define NUM_INODES_POINTERS 13
#define FILENAME_LENGTH 16
#define FILENAME_EXTENSION 3
#define MAX_NUMBER_FILES_IN_DIRECTORY 100
int unusedBlocks[1000];
int mapDisk[1000][1000];
int currentDirectoryPointer;

struct super_Block
{
    char magic[10];
    int blockSize;
    float FileSystemSize;
    int iNodeTableLength;
    int rootDirectoryNumber;
}superBlock;

struct i_node
{
    int mode;
    int linkCount;
    int uid;
    int gid;
    int size;
    int pointers[NUM_INODES_POINTERS];
};

struct directory
{
		int inodeId;
		char file[FILENAME_LENGTH + FILENAME_EXTENSION];
		char filename[FILENAME_LENGTH];
		char fileExtension[FILENAME_EXTENSION];
};

struct fileDescriptor{

	int isOpen;
	int pointer;

};

struct directory rootDirectory[MAX_NUMBER_FILES_IN_DIRECTORY];
struct fileDescriptor FDTable[16];

int mksfs(int fresh){
    int i;
    

		//initializes superBlockBuffer, inodeTableBuffer, rootDirectoryBuffer
		unsigned char superBlockBuffer[BLOCK_SIZE];
		unsigned char inodeTableBuffer[BLOCK_SIZE * 16];
		unsigned char rootDirectoryBuffer[BLOCK_SIZE * 14];
		unsigned char bitmap[BLOCK_SIZE];

	

    if(fresh){ 	
       printf("Disk is fresh!\n");


			init_fresh_disk(FILENAME, BLOCK_SIZE, NUM_BLOCKS);//if it does not exist, create one fresh

			init_disk(FILENAME, BLOCK_SIZE, NUM_BLOCKS); //load disk
        


				struct i_node root = {777,1,0,0,0,{17,18,19,20,21,0,0,0,0,0,0,0,0}}; //initializes root
			
			/* superBlock Operations */
			//create superblock buffer
					struct super_Block superBlock = {"0xAABB0005",BLOCK_SIZE,NUM_BLOCKS,NUM_BLOCKS,0}; //initializes superBlock

			memset(superBlockBuffer, 0, BLOCK_SIZE); //reads from 0 to BLOCK_SIZE
		
			memcpy((void *) superBlockBuffer, (const void* *) &superBlock, sizeof(superBlock)); //copy block
        




		/* inode operations */
		struct i_node inodeTable[NUM_INODES];
		
		memset(inodeTableBuffer, 0, sizeof(inodeTableBuffer));
			
		//set root as 1st element in table
		memset(inodeTable, 0, sizeof(inodeTable));//initializes table with 0s

		
		//set root as first element of inode table
		inodeTable[superBlock.rootDirectoryNumber] = root;

		//make a copy of inode table
		memcpy((void *) inodeTableBuffer, &inodeTable, sizeof(inodeTable));
		
		//write inodeTable into disk with the copy
		write_blocks(1, 16, inodeTableBuffer);


		/* initialize root directory */
		struct directory rootDirectory[MAX_NUMBER_FILES_IN_DIRECTORY];
		memset(rootDirectory, 0, sizeof(rootDirectory));	
		
		//initializes rootDirectoryBuffer

		memset(rootDirectoryBuffer, 0, sizeof(rootDirectoryBuffer));

		memcpy((void *) rootDirectoryBuffer, (const void *)&rootDirectory, sizeof(rootDirectory));

		//write rootDirectory into disk with the copy
		write_blocks(17, 14, rootDirectoryBuffer);



		
		/*Initializes Bitmap */
		
		memset(bitmap, 0, sizeof(bitmap)); // initializes all with 0 (free)
		/*
			block 0 = superBlock
			block 1-16 = inodeTable
			block 17-31 = root Directory
		*/
		bitmap[0] = 1; //super block
		for(i = 1; i < 17; i++){ //blocks 1-16 inodeTable used
			bitmap[i] = 1;
		}
		for(i = 17; i <= 34; i++){ //blocks 17-34 rootDirectory used
			bitmap[i] = 1;
		}

		bitmap[BLOCK_SIZE-1] = 1; //free bitmap	
		write_blocks(BLOCK_SIZE-1, 1, bitmap); //store bitmap at last block


		currentDirectoryPointer = 0; //currently points to the root

		struct fileDescriptor FDTable = {1,0};
		memset((void *)&FDTable, 0, sizeof(FDTable));



		/* TESTING */
		printf("superBlock Size = %lu \n", sizeof(superBlock));

		printf("SuperBlockBuffer Size = %lu \n", sizeof(superBlockBuffer));

		printf("inodeTableBuffer Size = %lu \n", sizeof(inodeTableBuffer));
		printf("inodeTable Size = %lu \n", sizeof(inodeTable));
		printf("rootDirectoryBuffer Size = %lu \n", sizeof(rootDirectoryBuffer));

	


    }else{ //if it already exists
        init_disk(FILENAME, BLOCK_SIZE, NUM_BLOCKS);
        
				//reads and puts in memory
				read_blocks(0, 1, &superBlock);
				read_blocks(1, 16, 	inodeTableBuffer);
				read_blocks(17, 34, rootDirectoryBuffer);
				read_blocks(NUM_BLOCKS-1, 1, bitmap);
        
				memset(FDTable,0, sizeof(FDTable));
        
    }
    
    
    return 0; // -1 if failed
}
int sfs_fopen(char *name){
	int i = 0;
	int fileFound = 0;
	
	
	for(i = 0; i < MAX_NUMBER_FILES_IN_DIRECTORY; i++){ //search in whole directory to see if file exists
			if(strncmp(rootDirectory[i].filename, name, sizeof(name)) == 0){ //if the filename exists
					if(FDTable[i].isOpen == 1){ //if the file is already opened
						printf("file is already opened");
						return -1;
					}else{ //open the existing file
						FDTable[i].isOpen = 1;

					}
			}
	}

	if(fileFound == 0){ //if no file is found
		//TODO : if the directory has less than the maximum file number allowed
		//find next available free block
		//store file there
		

	}
	
    return 0;
}


int sfs_fclose(int fileID){


		if(FDTable[fileID].isOpen == 1){ //if the file is open
			FDTable[fileID].isOpen = 0;	//set the file to close
		}	else{
			return -1; //if file is not even opened
		}


    return 0;
}

int sfs_fwrite(int fileID, const char *buf, int length){


		//look into the root directory for the file ID
		//find the index of file in the root
		//if file not found, return -1
		//find the last character in the datablock (read_block)
		//add the data following that index (write_block)
    return 0;
}
int sfs_fread(int fileID, char *buf, int length){
		
		//look into the root directory for the file ID
		//if file not found, return -1
		//
		


    return 0;
}


int sfs_fseek(int fileID, int offset){
		//return -1 if file not found
    return 0;
}
int sfs_remove(char *file){
	int i = 0;
			/*
			1 - remove the entry from the root directory
			2 - set datablocks to 0
			3 - 

			*/

			for(i = 0; i < MAX_NUMBER_FILES_IN_DIRECTORY; i++){ //search in whole directory to see if file exists
			if(strncmp(rootDirectory[i].filename, file, sizeof(file)) == 0){ //if the file exists
					if(FDTable[i].isOpen == 1){ //if the file is already opened
						FDTable[i].isOpen = 0; //close the file
						
					}
			}
	}



    return 0;
}
int sfs_get_next_filename(char* filename){
	char *nextFilename = "";
	int j = 0;
	int i = 0;

		for(i = 0; i < MAX_NUMBER_FILES_IN_DIRECTORY; i++){ //search in whole directory to see if file exists
			if(strncmp(rootDirectory[i].filename, filename, sizeof(filename)) == 0){ //if the file exists
					
				//initial file found, now search on the next ones only
					for( j = i; j < MAX_NUMBER_FILES_IN_DIRECTORY; j++){
						if(strncmp(rootDirectory[j].filename, "", sizeof(filename)) < 0){
							nextFilename = rootDirectory[j].filename;
							return 0;
						}

					}

			}else{
				return -1; //input file does not exist
				
			}
		}
	printf("nextFileName : %s", nextFilename);
	return 0;
}

int sfs_GetFileSize(const char* path){
    return 0;
}


/*
int main()
{
    printf("Hello World\n");
    mksfs(1);
    //sfs_fopen(FILENAME);
    //mksfs(1);
		return 0;
}

*/

