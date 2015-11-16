#include "2014021_2014012_FileSystemAPI.h"
#include "2014021_2014012_DiagnosticAPI.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main()
{
	int file_descriptor=createSFS("potato.dat",1024*1024*1);
	char *data,*beta;
	int ret;
	if(file_descriptor>0)
	{
		printf("1MB HDD created! with file descriptor %d\n",file_descriptor);
		data=(char*)malloc(sizeof(char*)*4*1024); //4KB block
		beta=(char*)malloc(sizeof(char*)*4*1024); //4KB block
		strcpy(data,"aloopotato");
		printf("%d\n",writeFile(file_descriptor,"wolo.txt",(void*)data));
		// //Write block to memory:
		// writeData(file_descriptor,0,(void*)data);

		// //Read that block from meory:
		// readData(file_descriptor,0,(void*)beta);
		// printf("Fetched block : %s\n",beta);
	}
	else
	{
		printf("Error!\n");
	}
}