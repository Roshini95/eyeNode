#include <2014021_2014012_FileSystemAPI.h>
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
	int return_value=open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR); //Add O_CREAT
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
	/*Return values :
	-1 : Error in lseek()
	-2 : Error in read()
	-3 : No space to write data
	-4 : No space to write inode entry
	-5 : Error writing to file
	 0 : File created successfully
	*/
	
	//Calculate actual_size and block_size by checking block until null (or any other method that works)

	int atcual_size,block_size; //Set this equal to the number of blocks required by the given *block.
	int j,i,k,blockNum,fourKB,data_space,inode_space;
	fourKB=4*1024;

	//Temporary :
	block_size=1;
	atcual_size=fourKB;

	data_space=-1;
	char inode_space_block,data_space_block;
	inode_space=-1;
	char* dataBitmap;
	blockNum=dataBitmapOffset;
	dataBitmap=(char*)malloc(sizeof(char)*fourKB); //4KB data bitmap
	if(lseek(disk,blockNum*fourKB,SEEK_SET)<0) return -1;
	if(read(disk,(void*)dataBitmap,fourKB) == -1){
		// printf("Data Bitmap%s\n", (char*)dataBitmap);
		return -2;
	}
	printf("First checkpoint\n");
	//Find space in data_bitmap
	int maxContFound = 0;
	for(i = 0; i < fourKB && maxContFound < block_size; i++)
	{
		for(j = 0; j < 8 && maxContFound < block_size; j++)
		{
			k = dataBitmap[i];
			int isSet = (k >> j) & 1;
			if (isSet == 0) maxContFound++;
			else maxContFound = 0;
		}
	}
	if (maxContFound >= block_size){
		data_space = (8 * i + j) - maxContFound;
		data_space_block = dataBitmap[ data_space / 8];
	}

	if(data_space == -1) return -3; //No space for data
	
	//Find space for inode entry in inode_bitmap
	char* inodeBitmap;
	blockNum=inodeBitmapOffset;
	inodeBitmap=(char*)malloc(sizeof(char)*fourKB); //4KB inode bitmap
	if(lseek(disk,blockNum*fourKB,SEEK_SET)<0) return -1;
	if(read(disk,(void*)inodeBitmap,fourKB)!=fourKB) return -2;
	printf("Second checkpoint\n");
	for(i=0;i<fourKB;i++)
	{
		for(j=0;j<8;j++)
		{
			k=(inodeBitmap[i]);
			k=(k>>j)&1;
			if(k)
			{
				inode_space=(8*i+j); 
				inode_space_block=inodeBitmap[inode_space/8]; //Get 8 bytes
				goto hell; 
			}
		}
	}	
	char yoda;
	hell:
	if(inode_space!=-1) return -4; //No space for inode entry

	//Set inode bitmap (inode_space)  to one:
	yoda=inode_space_block;
	yoda=(1<<(inode_space%8)) | yoda; //Setting 'inode_space%8'th bit
	if(lseek(disk,inodeBitmapOffset+(inode_space/8),SEEK_SET)<0) return -1; 
	if(write(disk,(void*)(&yoda),1)!=1) return -2; //Rewriting that whole byte (as it is tedious to rewrite individual bit)
	printf("Third checkpoint\n");

	//Set data bitmap (data_space) to one:
	yoda=data_space_block;
	yoda=(1<<(data_space%8)) | yoda; //Setting 'data_space%8'th bit
	if(lseek(disk,inodeBitmapOffset+(data_space/8),SEEK_SET)<0) return -1; 
	if(write(disk,(void*)(&yoda),1)!=1) return -2; ////Rewriting that whole byte (as it is tedious to rewrite individual bit)
	printf("Fourth checkpoint\n");

	//Write metadata to inode table
	if(lseek(disk,inodeDataOffset+inode_space*fourKB,SEEK_SET)<0) return -1;
	if(write(disk,(void*)filename,8)!=8) return -2; //Setting 8 byte filename
	printf("Fifth checkpoint\n");
	//Assuming write() shifts pointer to end of written block
	char* ex;
	ex=(char*)malloc(sizeof(char)*2);
	memcpy((void*)ex,(void*)(&data_space),2); //Copying starting block of file (data)
	if(write(disk,(void*)ex,2)!=2) return -2; //Setting 2 byte starting block of file
	printf("Sixth checkpoint\n");
	//free(ex);
	memcpy((void*)ex,(void*)(&block_size),2); //Copying number of blocks for file (data)
	if(write(disk,(void*)ex,2)!=2) return -2; //Setting 2 byte size of file (in blocks)
	printf("Seventh checkpoint\n");
	//free(ex);
	ex=(char*)malloc(sizeof(char)*4);
	memcpy((void*)ex,(void*)(&atcual_size),4); //Copying actual file size (data)
	if(write(disk,(void*)ex,4)!=4) return -2; //Setting 4 blocks for file size
	printf("Eigth checkpoint\n");

	//Write data
	char* one_block;
	one_block=(char*)malloc(sizeof(char)*fourKB);
	if(lseek(disk,dataOffset+(data_space*fourKB),SEEK_SET)<0) return -1; //Seek pointer to location where data is to be written
	printf("Ninth checkpoint\n");

	//Write 4KB at at time
	for(i=0;i<block_size;i++)
	{
		memcpy(one_block,((char*)block),fourKB);
		if(writeData(disk,(dataOffset/fourKB)+i+data_space,(void*)one_block)<0) return -5; //Error writing to file
	}
	return 0;
}

int readFile(int disk, char* filename, void* block){
	/*Return values :
	-1 : Error in lseek()
	-2 : Error in read()
	-3 : File not found error
	-4 : Error reading from file
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
	if(!found) return -3;
	int starting_block=((starting)*fourKB)+dataOffset;
	if(lseek(disk,(starting_block),SEEK_SET)<0) return -1; //Seek to starting of file
	char* one_block;
	one_block=(char*)malloc(sizeof(char)*fourKB);
	//Reading 4KB at a time :
	for(i=0;i<n_blocks;i++)
	{
		if(readData(disk,(dataOffset/fourKB)+i+starting_block,(void*)one_block)<0) return -4;
		memcpy((void*)(block+i*fourKB),(void*)one_block,fourKB); //Adding data to block, 4KB at a time
	}
	return 0;
}