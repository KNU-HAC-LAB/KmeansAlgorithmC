/*
 * GraphLocation.c
 * 각각의 데이터 포인터들을 그래픽으로 보여주기
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include "clustering.h"
#include "graphLocation.h"
#include "terminal.h"

#define STR_MAX 1024

#define TERMINAL_X_MAX 300
#define TERMINAL_Y_MAX 100

void figurePointing(const Point* data, const int data_arr_max, const int n_cluster)
{
	int i, j;
	int coloring = 1;
	float temp_x = 0.0, temp_y = 0.0;

	terminalSize(TERMINAL_X_MAX, TERMINAL_Y_MAX);

	for (i = 0; i < data_arr_max; i++)
	{
		temp_x = data[i].x;
		temp_y = data[i].y;
		if (temp_x < 1.0 && temp_x > 0.0)
			temp_x *= 10;
		if (data[i].y < 1.0 && data[i].y > 0.0)
			temp_y *= 10;
		gotoxy(temp_x + 10, temp_y);
		for (j = 0; j < n_cluster; j++)
		{
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