#include ".2014021_2014012.Misc.h"
#include <stdio.h>
#include <stdlib.h>

void makeDisk(int disk,int nBytes)
{
	FILE * fp;
	fp=fopen("putatoh.txt","w+");
	int i=0;
	for(i=0;i<nBytes;i++)
	{
		fprintf(fp,"0");	
	}
}