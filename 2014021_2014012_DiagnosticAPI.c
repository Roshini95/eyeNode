#include "2014021_2014012_DiagnosticAPI.h"

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
	
}