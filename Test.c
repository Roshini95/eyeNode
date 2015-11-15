#include "2014021_2014012_FileSystemAPI.h"
#include <stdio.h>

void main()
{
	int file_descriptor=createSFS("potato.dat",1024*1024*128);
	if(file_descriptor>0)
	{
		printf("128MB HDD created!\n");
	}
	else
	{
		printf("Error!\n");
	}
}