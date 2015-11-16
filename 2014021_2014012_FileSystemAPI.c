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

int createSFS(char* filename, int nbytes){
	/*Return values :
	-1 : File not created
	-2 : Error while writing to file 
	+ve value : File created as expected
	*/
	int return_value=open(filename, O_RDWR, S_IRUSR | S_IWUSR); //Add O_CREAT
	if(return_value<0) return -1;
	int i,err;
	char data=0;
	for(i=0;i<nbytes;i++)
	{
		err=write(return_value,(void*)(&data),1);
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
	int fourKB=4*1024;
	if(lseek(disk,blockNum*fourKB,SEEK_SET)<0) return -1;
	if(read(disk,block,fourKB)!=fourKB) return -2; //4KB Data Block
	return fourKB;
}

int writeData(int disk, int blockNum, void* block){
	/*Return values :
	-1 : Error in lseek()
	-2 : Error in write()
	 +ve number : Data written successfully
	*/
	 int fourKB=4*1024;
	 if(lseek(disk,blockNum*fourKB,SEEK_SET)<0) return -1;
	 if(write(disk,block,fourKB)!=fourKB) return -2; //4KB Data Block
	 return fourKB;
}

int writeFile(int disk, char* filename, void* block){
	//Assume block is 4KB for now
	int j,i,k,blockNum,bytes_read,data_space,inode_space;
	bytes_read=4*1024;
	data_space=-1;
	inode_space=-1;
	char* dataBitmap;
	blockNum=dataBitmapOffset;
	dataBitmap=(char*)malloc(sizeof(char)*bytes_read); //4KB data bitmap
	if(lseek(disk,blockNum*fourKB,SEEK_SET)<0) return -1;
	if(read(disk,(void*)dataBitmap,bytes_read)!=bytes_read) return -2;
	if(lseek(disk,0,SEEK_SET)<0) return -1; //Return to starting of HDD
	//Find space in data_bitmap
	for(i=0;i<bytes_read;i++)
	{
		for(j=0;j<8;j++)
		{
			k=(dataBitmap[i]);
			k=(k>>j)&1;
			if(k)
			{
				data_space=(8*i+j);
				goto heaven; 
			}
		}
	}
	heaven:
	if(data_space==-1) return -3; //No space for data
	//Find space for inode entry in inode_bitmap
	char* inodeBitmap;
	blockNum=inodeBitmapOffset;
	inodeBitmap=(char*)malloc(sizeof(char)*bytes_read); //4KB inode bitmap
	if(lseek(disk,blockNum*fourKB,SEEK_SET)<0) return -1;
	if(read(disk,(void*)inodeBitmapOffset,bytes_read)!=bytes_read) return -2;
	if(lseek(disk,0,SEEK_SET)<0) return -1; //Return to starting of HDD
	for(i=0;i<bytes_read;i++)
	{
		for(j=0;j<8;j++)
		{
			k=(dataBitmap[i]);
			k=(k>>j)&1;
			if(k)
			{
				inode_space=(8*i+j);
				goto hell; 
			}
		}
	}	
	hell:
	if(inode_space!=-1) return -4; //No space for inode entry
	//Set inode bitmap to one:

	if(lseek(disk,0,SEEK_SET)<0) return -1; //Return to starting of HDD
	if(write(disk,(void*)))
	//Set data bitmap to one:

	//Write metadata to inode table

	//Write data
}

int readFile(int disk, char* filename, void* block){
	/*Return values :
	-1 : Error in lseek()
	-2 : Error in read()
	 0 : Data read successfully
	*/
	int i,found;
	int fourKB=1024*4;
	int starting,file_size,n_blocks;
	char *name,*word;
	word=(char*)malloc(sizeof(char)*16);
	name=(char*)malloc(sizeof(char)*8); 
	found=0;
	for(i=inodeDataOffset;i<dataOffset;i+=16)
	{
		if(lseek(disk,i,SEEK_SET)<0) return -1;
		if(read(disk,(void*)word,16)!=16) return -2; //4KB Data Block
		memcpy((void*)name,(void*)word,8); //Extract file name
		if(strcmp(name,filename) == 0)
		{
			found=1;
			memcpy((void*)(&starting),(void*)(word+8),2);
			memcpy((void*)(&n_blocks),(void*)(word+10),2);
			memcpy((void*)(&file_size),(void*)(word+12),4);
			break;
		}
	}	
	if(!found)
	{
		printf("File not found!\n");
		return -3;
	}
	else
	{
		int starting_block=((starting)*fourKB)+dataOffset;
		if(lseek(disk,(starting_block),SEEK_SET)<0) return -1; //Seek to starting of file
		if(read(disk,block,(file_size))!=(file_size)) return -2; //Read 'size' bytes
	}
	return 0;
}