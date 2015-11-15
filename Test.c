#include "2014021_2014012_FileSystemAPI.h"
#include <stdio.h>

void main()
{
	int file_descriptor=createSFS("potato.dat",1024*1024*1);
	if(file_descriptor>0)
	{
		printf("1MB HDD created! with file descriptor %d\n",file_descriptor);
		void* potato;
		printf("%d\n",readData(file_descriptor,0,potato));
	}
	else
	{
		printf("Error!\n");
	}

}