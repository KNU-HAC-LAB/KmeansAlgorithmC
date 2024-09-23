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
void clustering(Point* data, const int point_arr_max, const Centroid* centroid, const int n_cluster)
{
	int i, j, k;
	float min = MAX_NUM_FOR_MIN;
	int numbering = 0;			// 몇번째 centroid 인지
	float* centroid_clustering = (float*)calloc(n_cluster, sizeof(float));

	/*for (i = 0; i < point_arr_max; i++)
	{
		for (j = 0; j < n_cluster; j++)
			printf("i: %d, j: %d, data[i].x: %f, data[i].y: %f, centroid[j].x: %f, centroid[j].y: %f\n"
				, i, j, data[i].x, data[i].y, centroid[j].x, centroid[j].y);
	}*/

	if (centroid_clustering == NULL) return;

	for (i = 0; i < point_arr_max; i++)
	{
		printf("i: %d\n", i);
		for (j = 0; j < n_cluster; j++)
		{
			printf("j: %d\n", j);
			centroid_clustering[j] = euclideanDistance(data[i], centroid[j]);
			printf("centroid_clustering[j]: %f\n"
				, centroid_clustering[j]);
		}

		printf("\n");
		for (j = 0; j < n_cluster; j++)
		{
			printf("j: %d\n", j);
			if (centroid_clustering[j] < min)
			{
				min = centroid_clustering[j];
				numbering = j;
			}
			printf("j: %d, centroid_clustering[j]: %f, min: %f\n"
				, j, centroid_clustering[j], min);
		}
		
		min = MAX_NUM_FOR_MIN;
		data[i].centroid_num = numbering;
	}

	free(centroid_clustering);
}

// 유클리드 거리 계산
float euclideanDistance(Point data, Centroid centroid)
{
	printf("data.x: %f, data.y: %f, centroid.x: %f, centroid.y: %f\n", data.x, data.y, centroid.x, centroid.y);
	return sqrt(pow(data.x - data.y, 2) + pow(centroid.x + centroid.y, 2));
}