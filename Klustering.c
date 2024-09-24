#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "klustering.h"

#define MAX_NUM_FOR_MIN 100

// CSV에 있는 데이터를 배열로 옮기기
void dataToArray(const char* str_tmp, const int x, const int y, Point* data, const int index)
{
	char* token = strtok(str_tmp, ",");
	float pointX = 0.0, pointY = 0.0;
	int i = 0;

	while (token != NULL)
	{
		if (i == x)
			data[index].x = atof(token);
		if (i == y)
			data[index].y = atof(token);

		token = strtok(NULL, ",");
		i++;
	}
	i = 0;
}

// 클러스터링 코드
void clustering(Point* data, const int data_arr_max, const Centroid* centroid, const int n_cluster)
{
	int i, j, k;
	float min = MAX_NUM_FOR_MIN;
	int numbering = 0;														// 몇번째 centroid 인지
	float* centroid_clustering = (float*)calloc(n_cluster, sizeof(float));	// 유클리드 거리 계산 후 저장하기

	/*for (i = 0; i < data_arr_max; i++)
	{
		for (j = 0; j < n_cluster; j++)
			printf("i: %d, j: %d, data[i].x: %f, data[i].y: %f, centroid[j].x: %f, centroid[j].y: %f\n"
				, i, j, data[i].x, data[i].y, centroid[j].x, centroid[j].y);
	}*/

	if (centroid_clustering == NULL) return;

	for (i = 0; i < data_arr_max; i++)
	{
		for (j = 0; j < n_cluster; j++)
		{
			centroid_clustering[j] = euclideanDistance(data[i], centroid[j]);
			/*printf("centroid_clustering[j]: %f\n"
				, centroid_clustering[j]);*/
		}

		for (j = 0; j < n_cluster; j++)
		{
			//printf("second. j: %d\n", j);
			if (centroid_clustering[j] < min)
			{
				min = centroid_clustering[j];
				numbering = j;
			}
			/*printf("centroid_clustering[j]: %f, min: %f, numbering: %d\n"
				, centroid_clustering[j], min, numbering);*/
		}
		
		min = MAX_NUM_FOR_MIN;
		data[i].centroid_num = numbering;
	}

	free(centroid_clustering);
}

// 유클리디안 거리 계산
float euclideanDistance(Point data, Centroid centroid)
{
	/*printf("data.x: %f, data.y: %f, centroid.x: %f, centroid.y: %f\n", data.x, data.y, centroid.x, centroid.y);*/
	return sqrt(pow(data.x - centroid.x, 2) + pow(data.y - centroid.y, 2));
}

// 중심으로 위치 갱신
void toCentroidCenter(Point* data, const int data_arr_max, Centroid* centroid, const int n_cluster)
{
	Point* centroid_to_center = (Point*)calloc(n_cluster, sizeof(Point));
	int i;

	if (centroid_to_center == NULL) return -1;

	for (i = 0; i < data_arr_max; i++)
	{
		centroid_to_center[data[i].centroid_num].x += data[i].x;
		centroid_to_center[data[i].centroid_num].y += data[i].y;
		centroid_to_center[data[i].centroid_num].centroid_num++;

		/*printf("%d, data[i].x: %f, centroid_to_center[%d].x: %f, data[i].y: %f, centroid_to_center[%d].y : %f\n",
			i, data[i].x, data[i].centroid_num, centroid_to_center[data[i].centroid_num].x, data[i].y, data[i].centroid_num, centroid_to_center[data[i].centroid_num].y);*/
	}

	for (i = 0; i < n_cluster; i++)
	{
		centroid[i].x = centroid_to_center[i].x / centroid_to_center[i].centroid_num;
		centroid[i].y = centroid_to_center[i].y / centroid_to_center[i].centroid_num;
	}

	free(centroid_to_center);
}