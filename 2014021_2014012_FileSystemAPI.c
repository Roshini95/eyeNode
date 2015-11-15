#include "2014021_2014012_FileSystemAPI.h"
#include <unistd.h> 
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
	File System Offsets	in Bytes
*/
#define superOffset  0 * 1024
#define inodeBitmapOffset   4 * 1024
#define dataBitmapOffset  8 * 1024
#define inodeDataOffset  12 * 1024
#define dataOffset  ( 12 + 4 * 128 ) * 1024

int createSFS( char* filename, int nbytes){
	/*Return values :
	-1 : File not created
	-2 : Error while writing to file 
	+ve value : File created as expected
	*/
	int return_value=open(filename, O_RDWR, S_IRUSR | S_IWUSR);
	if(return_value<0) return -1;
	int i,err;
	for(i=0;i<nbytes;i++)
	{
		err=write(return_value,"0",1);
		if(err!=1) return -2;
	}
	return return_value;	
}

int readData(int disk, int blockNum, void* block)
{
	/*Return values :
	-1 : Error in lseek()
	-2 : Error in read()
	 +ve number : Data read successfully
	*/
	int bytes_read=4*1024;
	if(lseek(disk,blockNum*1024*4,SEEK_SET)<0) return -1;
	if(read(disk,block,bytes_read)!=bytes_read) return -2; //4KB Data Block
	lseek(disk,0,SEEK_SET); //Return back to starting of file
	return bytes_read;
}

int writeData(int disk, int blockNum, void* block){
	/*Return values :
	-1 : Error in lseek()
	-2 : Error in write()
	 +ve number : Data written successfully
	*/
	 int bytes_to_write=4*1024;
	 if(lseek(disk,blockNum*1024*4,SEEK_SET)<0) return -1;
	 if(write(disk,block,bytes_to_write)!=bytes_to_write) return -2; //4KB Data Block
	 lseek(disk,0,SEEK_SET); //Return back to starting of file
	 return bytes_to_write;
}

int writeFile(int disk, char* filename, void* block){
	int exists = 0;
	int i,active;
	int j = inodeBitmapOffset;
	for (i = inodeDataOffset; i < dataOffset; i += 16){
		// Check if inode is active
		if ( lseek(disk, j, SEEK_SET) >= 0 ){
			char* tempCheck;
			tempCheck=(char*)malloc(sizeof(tempCheck));
			if (read(disk, (void*)tempCheck, 1) == 1){
				int x = (*tempCheck);
				// Extract the first bit of x[which is of 8 bits] and check whether it is set
				active = x & (1 << 7);
				if (active){
					char *fileNameCheck;
					fileNameCheck=(char*)malloc(sizeof(char*)*8);
					lseek(disk, i, SEEK_SET);
					read(disk, (void*)fileNameCheck, 8);
					if (strcmp(fileNameCheck, filename) == 0){
						// If space is still left for block in file, write in file
						// Else, copy entire file to buffer, and erase this location

					}
				}
				else{
					// Inactive INode found: Try to write data

				}
			}
			else{
				printf("Error Reading Inode Bitmap\n");
			}
		}
		else{
			printf("Error Seeking to Inode Bitmap\n");
		}
	}	
}

int readFile(int disk, char* filename, void* block){
	/*Return values :
	-1 : Error in lseek()
	-2 : Error in read()
	 0 : Data read successfully
	*/
	int i,found;
	int* starting,*file_size,*n_blocks;
	starting=(int*)malloc(sizeof(starting));
	file_size=(int*)malloc(sizeof(file_size));
	n_blocks=(int*)malloc(sizeof(n_blocks));
	char *name,*word;
	word=(char*)malloc(sizeof(char*)*16);
	name=(char*)malloc(sizeof(char*)*8); 
	found=0;
	for(i=inodeDataOffset;i<dataOffset;i+=16)
	{
		if(lseek(disk,i,SEEK_SET)<0) return -1;
		if(read(disk,(void*)word,16)!=16) return -2; //4KB Data Block
		memcpy((void*)name,(void*)word,8); //Extract file name
		if(!strcmp(name,filename))
		{
			found=1;
			memcpy((void*)starting,(void*)(word+8),2);
			memcpy((void*)n_blocks,(void*)(word+10),2);
			memcpy((void*)file_size,(void*)(word+12),4);
			break;
		}
		lseek(disk,0,SEEK_SET); //Return back to starting of file
	}	
	if(!found)
	{
		printf("File not found!\n");
		return -3;
	}
	else
	{
		int starting_block=((*starting)*4*1024)+dataOffset;
		lseek(disk,0,SEEK_SET); //Return back to starting of file
		if(lseek(disk,(*starting),SEEK_SET)<0) return -1; //Seek to starting of file
		if(read(disk,block,(*file_size))!=(*file_size)) return -2; //Read 'size' bytes
	}
	return 0;
}