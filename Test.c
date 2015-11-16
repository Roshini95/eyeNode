#include <2014021_2014012_FileSystemAPI.h>
#include <2014021_2014012_DiagnosticAPI.h>	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main()
{
	int file_descriptor=createSFS("test.dat",1024*1024*1);
	printf("Here\n");
	char *data,*beta;
	int ret;
	if(file_descriptor>0)
	{
		printf("1MB HDD created! with file descriptor %d\n",file_descriptor);
		data=(char*)malloc(sizeof(char*)*4*1024); //4KB block
		beta=(char*)malloc(sizeof(char*)*4*1024); //4KB block
		strcpy(data,"potato tamatar");
		printf("%d\n",writeFile(file_descriptor,"abcdefgh",(void*)data));
		printf("Reading file\n");
		strcpy(data, "Reset");

		printf("%d\n",readFile(file_descriptor,"abcdefgh",(void*)data));
		printf("Data : %s\n",data);
		printf("Debugging:\n");
		// print_inodeBitmaps(file_descriptor);
		// print_dataBitmaps(file_descriptor);
		print_FileList(file_descriptor);
	}
	else
	{
		printf("Error!\n");
	}
}