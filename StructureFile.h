#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct Student{
	int id;
	char name[32];
}Student;

typedef struct DataPage{
	Student data[3];
	char unused[20];
}DataPage;

typedef struct IndexPage{
	int pageID[16];
	int key[15];
	char unused[4];
}IndexPage;
