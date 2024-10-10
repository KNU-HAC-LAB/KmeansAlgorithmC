/*
 * GraphLocation.c
 * 각각의 데이터 포인터들을 그래픽으로 보여주기
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clustering.h"
#include "graphLocation.h"
#include "terminal.h"

#define STR_MAX 1024

#define TERMINAL_X_MAX 100
#define TERMINAL_Y_MAX 100

void figurePointing(const char* csv_name, const Point* data, const int data_arr_max, const int n_cluster)
{
	int i, j;
	int coloring = 1;

	terminalSize(TERMINAL_X_MAX, TERMINAL_Y_MAX);

	for (i = 0; i < data_arr_max; i++)
	{
		for (j = 0; j < n_cluster; j++)
		{
			if (!strcmp(csv_name, "iris.csv"))
				gotoxy(data[i].x * 10, data[i].y * 10);
			else
				gotoxy(data[i].x, data[i].y);

			if (data[i].centroid_num == j)
			{
				textColoring(coloring);
				printf("○");
			}
			coloring++;
		}
		coloring = 1;
	}
	gotoxy(0, TERMINAL_Y_MAX - 1);
	textColoring(15);
}