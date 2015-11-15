#include "2014021_2014012_FileSystemAPI.h"
#include <unistd.h> 
#include <fcntl.h>

int createSFS( char* filename, int nbytes)
{
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

}

int writeData(int disk, int blockNum, void* block)
{

}

int writeFile(int disk, char* filename, void* block)
{

}

int readFile(int disk, char* filename, void* block)
{
	
}