#define _CRT_SECURE_NO_WARNINGS
#define INDEX_PAGE_SIZE 16
#define PAGE_SIZE 128
#define RECORDS_PER_PAGE 3
#define RECORDS 768


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "StructureFile.h"

void createIndexPage(char *fileName){

	int location = 0;

	for (int i = 0; i <= RECORDS / RECORDS_PER_PAGE / INDEX_PAGE_SIZE; ++i){

		IndexPage ip;

		FILE *bfp = fopen(fileName, "rb");

		for (int i = 0; i < INDEX_PAGE_SIZE - 1; ++i){
			ip.pageID[i] = location * PAGE_SIZE;
			fseek(bfp, ip.pageID[i], SEEK_SET);
			fread(&(ip.key[i]), sizeof(int), 1, bfp);

			location += 1;
		}

		fclose(bfp);

		bfp = fopen(fileName, "ab");

		fwrite(&ip, sizeof(IndexPage), 1, bfp);

		fclose(bfp);
	}
}

void readFromBinaryFile(char *fileName){
	FILE *fp = fopen(fileName, "rb");

	DataPage dp1;

	while (fread(&dp1, sizeof(DataPage), 1, fp) == 1){
		for (int k = 0; k < 3; ++k)
			printf("%d %s\n", dp1.data[k].id, dp1.data[k].name);
	}

	fclose(fp);
}

void dumpToBinaryFile(char *inputFileName, char *outputFileName){
	FILE *fp = fopen(inputFileName, "r");

	FILE *bfp = fopen(outputFileName, "wb");

	fclose(bfp);

	bfp = fopen(outputFileName, "ab");

	char *line = (char *)malloc(sizeof(char)* PAGE_SIZE);

	int i = 0, p = 0;

	DataPage dp;

	fgets(line, PAGE_SIZE, fp);

	while (fgets(line, PAGE_SIZE, fp) != '\0'){
		dp.data[i].id = atoi(strtok(line, ","));
		strcpy(dp.data[i].name, strtok(NULL, ","));
		i += 1;
		p += 1;
		if (i == RECORDS_PER_PAGE){
			fwrite(&dp, sizeof(DataPage), 1, bfp);
			memset(&dp, 0, sizeof(DataPage));
			i = 0;
		}
		if (p == RECORDS){
			if (p % 3 != 0)
				fwrite(&dp, sizeof(DataPage), 1, bfp);
			break;
		}
	}

	fclose(bfp);

	fclose(fp);
}

int main(){

	dumpToBinaryFile("UserData.csv", "UserDataBinary.bin");

	//readFromBinaryFile("UserDataBinary.bin");

	createIndexPage("UserDataBinary.bin");

	system("pause");
	return 0;
}