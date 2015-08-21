#include "CSV _Load.h"
int stage_state[110];

int csv_file_load(char* name)
{
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, name, "r");
	if (fp == NULL){
		return -1;
	}


	//ì«Ç›çûÇ›
	for (int i = 0; i < 150; i++)
	{
		fscanf_s(fp, "%d \n", &stage_state[i]);
	}



	fclose(fp);

	return 0;
}