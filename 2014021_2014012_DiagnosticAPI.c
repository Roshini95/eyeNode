#include "2014021_2014012_DiagnosticAPI.h"
#include <stdio.h>

/*
	File System Offsets	in Bytes
*/
const int superOffset = 0 * 1024; 
const int inodeBitmapOffset =  4 * 1024; 
const int dataBitmapOffset = 8 * 1024;
const int inodeDataOffset = 12 * 1024;
const int dataOffset = ( 12 + 4 * 128 ) * 1024;

void print_inodeBitmaps(int fileSystemId)
{
	char* inode_map;
	int bytes_to_read=4*1024; //4KB
	if(lseek(disk,inodeBitmapOffset,SEEK_SET)<0) return ; //Error reaching offset
	if(read(disk,inode_map,bytes_read)!=bytes_read) return; //Error reading from file
	printf("Inode bitmap :\n%s\n",inode_map);
}

void print_dataBitmaps(int fileSystemId)
{
	char* data_map;
	int bytes_to_read=4*1024; //4KB
	if(lseek(disk,dataBitmapOffset,SEEK_SET)<0) return ; //Error reaching offset
	if(read(disk,data_map,bytes_read)!=bytes_read) return; //Error reading from file
	printf("Data bitmap :\n%s\n",data_map);
}

void print_FileList(int fileSystemId)
{	
	char* file_name,*starting_block,*number_of_blocks,*file_size;
	char* inode_map,*inode_data;
	int bytes_read=4*1024*128; //128 blocks for inode
	if(lseek(disk,inodeBitmapOffset,SEEK_SET)<0) return ; //Error reaching offset
	if(read(disk,inode_map,bytes_read)!=bytes_read) return; //Error reading from file
	int i,j,k;
	if(lseek(disk,inodeDataOffset,SEEK_SET)<0) return ; //Error reaching second offset
	if(read(disk,inode_data,bytes_read)!=bytes_read) return; //Error reading second time from file
	//Print data for non-zero inode entries
	char* entry;
	for(i=0;i<4*1024;i++) 
	{	
		for(j=0;j<8;j++)
		{
			k=inode_map[i];
			k=(k>>j)&1;
			if(k)
			{
				substring(inode_data,entry,16*i,16); 
				substring(entry,file_name,0,8);
				substring(entry,starting_block,8,2);
				substring(entry,number_of_blocks,10,2);
				substring(entry,file_size,12,4);
				printf("File name : %s\n",file_name);
				printf("Starting block of file : %s\n",starting_block);
				printf("Number of blocks : %s\n",number_of_blocks);
				printf("File size : %s\n",file_size);
			}
		}		
	}	
}