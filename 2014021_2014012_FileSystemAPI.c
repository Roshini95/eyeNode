#include "2014021_2014012_FileSystemAPI.h"
#include <unistd.h> 
#include <fcntl.h>
#include <sys/types.h>

/*
	File System Offsets	in Bytes
*/
const int superOffset = 0 * 1024; 
const int inodeBitmapOffset =  4 * 1024; 
const int dataBitmapOffset = 8 * 1024;
const int inodeDataOffset = 12 * 1024;
const int dataOffset = ( 12 + 4 * 128 ) * 1024;

int createSFS( char* filename, int nbytes){
	/*Return values :
	-1 : File not created
	-2 : Error while writing to file 
	+ve value : File created as expected
	*/
	int return_value=open(filename, O_WRONLY | O_CREAT);
	if(return_value<0) return -1;
	int i,err;
	for(i=0;i<nbytes;i++)
	{
		err=write(return_value,"0",1);
		if(err!=1) return -2;
	}
	return return_value;	
}

int readData( int disk, int blockNum, void* block)
{
	/*Return values :
	-1 : Error in lseek()
	-2 : Error in read()
	 +ve number : Data read successfully
	*/
	int bytes_read=4*1024;
	if(lseek(disk,blockNum*1024*4,SEEK_SET)<0) return -1;
	if(read(disk,(char*)block,bytes_read)!=bytes_read) return -2; //4KB Data Block
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
	 if(write(disk,(char*),bytes_to_write)!=bytes_to_write) return -2; //4KB Data Block
	 lseek(disk,0,SEEK_SET); //Return back to starting of file
	 return bytes_to_write;
}

int writeFile(int disk, char* filename, void* block){
	int exists = 0;
	int i;
	int j = inodeBitmapOffset;
	for (i = inodeDataOffset; i < dataOffset; i += 16){
		// Check if inode is active
		if ( lseek(disk, j, SEEK_SET) >= 0 ){
			char tempCheck;
			if (read(disk, &tempCheck, 1) == 1){
				uint8_t x = tempCheck;
				// Extract the first bit of x[which is of 8 bits] and check whether it is set
				active = x & (1 << 7);
				if (active){
					char fileNameCheck[8];
					lseek(disk, i, SEEK_SET);
					read(disk, fileNameCheck, 8);
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
	char* name,word,*starting,*file_size,*n_blocks;
	found=0;
	for(i=inodeDataOffset;i<dataOffset;i+=16)
	{
		if(lseek(disk,i,SEEK_SET)<0) return -1;
		if(read(disk,word,16)!=16) return -2; //4KB Data Block
		substring(word,name,0,8); //Extract file name
		if(!strcmp(name,filename))
		{
			found=1;
			substring(word,starting,8,2);
			substring(word,n_blocks,10,2);
			substring(word,file_size,12,4);
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
		uint8_t starting_block=(starting*4*1024)+dataOffset;
		uint8_t size_in_int=file_size;
		lseek(disk,0,SEEK_SET); //Return back to starting of file
		if(lseek(disk,starting,SEEK_SET)<0) return -1; //Seek to starting of file
		if(read(disk,(char*)block,size_in_int)!=size_in_int) return -2; //Read 'size' bytes
	}
	return 0;
}