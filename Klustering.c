#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include "klustering.h"

void dataToArray(const char* str_tmp, const int x, const int y, const int index)
{
	printf("str_tmp: %s\n", str_tmp);
	char* token = strtok(str_tmp, ",");
	/*float pointX = 0.0, pointY = 0.0;
	int i = 0;*/

	while (token != NULL)
	{
		/*if (i > 5)
			i = 0;
		else if (i == x)
			pointX = atof(token);
		else if (i == y)
			pointY = atof(token);*/
		
		printf("token: %s\n", token);
		token = strtok(NULL, ",");
		//i++;
	}
}