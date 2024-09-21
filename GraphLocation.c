#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include "graphLocation.h"
#include "terminal.h"

#define STR_MAX 1024

int selectTheXYData(const char* str_tmp)
{
	char* token = strtok(str_tmp, ",");
	int i = 0;

	while (token != NULL)
	{
		i++;
		printf("%d. %s\n", i, token);
		token = strtok(NULL, ",");
	};
	return 0;
}

void cordinatesInit(const int size)
{
	int i;
	
	terminalSizeInit(size * 2, size);

	for (i = 0; i < size - 1; i++)
		printf("¦­\n");
	printf("¦±");
	for (i = 2; i < size * 2; i++)
		printf("¦¬");
}

void figurePointing(const char* str_tmp)
{

}