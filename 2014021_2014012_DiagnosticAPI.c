#include "2014021_2014012_DiagnosticAPI.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
	File System Offsets	in Bytes
*/
#define superOffset  0 * 1024
#define inodeBitmapOffset   4 * 1024
#define dataBitmapOffset  8 * 1024
#define inodeDataOffset  12 * 1024
#define dataOffset  ( 12 + 4 * 128 ) * 1024

void print_inodeBitmaps(int fileSystemId)
{
	char* inode_map;
	int bytes_to_read=4*1024; //4KB
	inode_map=(char*)malloc(sizeof(char*)*bytes_to_read);
	if(lseek(fileSystemId,inodeBitmapOffset,SEEK_SET)<0) return ; //Error reaching offset
	if(read(fileSystemId,(void*)inode_map,bytes_to_read)!=bytes_to_read) return; //Error reading from file
	printf("Inode bitmap :\n%s\n",inode_map);
}

void print_dataBitmaps(int fileSystemId)
{
	char* data_map;
	int bytes_to_read=4*1024; //4KB
	data_map=(char*)malloc(sizeof(char*)*bytes_to_read);
	if(lseek(fileSystemId,dataBitmapOffset,SEEK_SET)<0) return ; //Error reaching offset
	if(read(fileSystemId,(void*)data_map,bytes_to_read)!=bytes_to_read) return; //Error reading from file
	printf("Data bitmap :\n%s\n",data_map);
}

void print_FileList(int fileSystemId)
{	
	char* file_name;
	int *starting_block,*number_of_blocks,*file_size;
	file_name=(char*)malloc(8*sizeof(char*)); //8-byte word filename
	starting_block=(int*)malloc(sizeof(starting_block));
	number_of_blocks=(int*)malloc(sizeof(number_of_blocks));
	file_size=(int*)malloc(sizeof(file_size));
	char* inode_map,*inode_data;
	int fourKB=4*1024;
	int bytes_read=fourKB*128; //128 blocks for inode
	inode_map=(char*)malloc(sizeof(char*)*fourKB);
	inode_data=(char*)malloc(sizeof(char*)*bytes_read);
	if(lseek(fileSystemId,inodeBitmapOffset,SEEK_SET)<0) return; //Error reaching offset
	if(read(fileSystemId,(void*)inode_map,bytes_read)!=bytes_read) return; //Error reading from file
	int i,j,k;
	if(lseek(fileSystemId,inodeDataOffset,SEEK_SET)<0) return ; //Error reaching second offset
	if(read(fileSystemId,(void*)inode_data,bytes_read)!=bytes_read) return; //Error reading second time from file
	//Print data for non-zero inode entries
	char* entry;
	entry=(char*)malloc(sizeof(char*)*16); //Size of an inode entry
	for(i=0;i<4*1024;i++) 
	{	
		for(j=0;j<8;j++)
		{
			k=(int)inode_map[i];
			k=(k>>j)&1;
			if(k)
			{
				//Reading (8i+j)th inode data entry
				memcpy((void*)entry,(void*)(inode_data+(8*i+j)*16),16); //16 bytes per inode entry
				memcpy((void*)file_name,(void*)entry,8); //First 8 bytes : filename
				memcpy((void*)starting_block,(void*)(entry+8),2); //Next 2 bytes : starting_block
				memcpy((void*)number_of_blocks,(void*)(entry+10),2); //Next 2 bytes : number of blocks
				memcpy((void*)file_size,(void*)(entry+12),4); //Next 4 bytes : file size
				printf("File name : %s\n",file_name);
				printf("Starting block of file : %d\n",*starting_block);
				printf("Number of blocks : %d\n",*number_of_blocks);
				printf("File size : %d\n",*file_size);
			}
		}		
	}	
}