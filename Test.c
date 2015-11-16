// #include <.2014021_2014012.FileSystemAPI.h>
// #include <.2014021_2014012.DiagnosticAPI.h>	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main()
{
	// char yo[]="chakke\0";
	// int pot;
	// void *block = (void*)malloc(100);
	// block = (void*)yo;
	// pot=0;
	// char* toy;
	// while (*(char*)(block+pot)!=0){
	// 	toy=(char*)(block+pot);
	// 	printf("%c",*toy);
	// 	pot++;
	// 	if(pot==16) break;
	// }
	
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