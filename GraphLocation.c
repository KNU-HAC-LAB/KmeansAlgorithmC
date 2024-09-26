#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#include "graphLocation.h"
#include "terminal.h"

#define STR_MAX 1024

int selectTheXYData(const char* str_tmp)
{
	char* token = strtok(str_tmp, ",");
	int i = 0, key, x = 12, y = 1;
	unsigned int select = 0;

	while (token != NULL)
	{
		i++;
		printf("%d. %s\n", i, token);
		token = strtok(NULL, ",");
	};

	gotoxy(x, y);
	printf("¡Ü");
	while (1)
	{
		if (_kbhit())
		{
			printf("\b\b  ");
			key = _getch();
			switch (key)
			{
			case 'w':
				gotoxy(x, y--);
				select--;
				break;
			case 's':
				gotoxy(x, y++);
				select++;
				break;
			case 13:
				printf("select: %d\n", select);
				return select;
			default:
				break;
			}
			printf("¡Ü");
		}
	}
}

void figurePointing(const char* str_tmp, const int x, const int y)
{
	char* token = strtok(str_tmp, ",");
	float pointX = 0.0, pointY = 0.0;
	int index = 0;

	while (token != NULL)
	{
		if (index > 5)
			index = 0;
		if (index == x)
			pointX = atof(token);
		if (index == y)
			pointY = atof(token);
		gotoxy(pointX * 10, pointY * 10);
		printf("¡Û");
		//printf("pointX: %f, pointY: %f\n", pointX, pointY);
		token = strtok(NULL, ",");
		index++;
	};
}