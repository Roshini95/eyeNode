#include "2014021_2014012_FileSystemAPI.h"
#include "2014021_2014012_DiagnosticAPI.h"
#include <stdio.h>

void main()
{
	int file_descriptor=createSFS("potato.dat",1024*1024*1);
	char* data;
	int ret;
	if(file_descriptor>0)
	{
		printf("1MB HDD created! with file descriptor %d\n",file_descriptor);
		ret=readData(file_descriptor,0,(void*)data);
		printf("Value returned : %d\n",ret);
	}
	else
	{
		printf("Error!\n");
	}
}