#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "klustering.h"

#define MAX_NUM_FOR_MIN 100

// CSV�� �ִ� �����͸� �迭�� �ű��
void dataToArray(
	const char* str_tmp,
	const int x,
	const int y,
	Point* data,
	const int index)
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

// Ŭ�����͸� �ڵ�
void clustering(
	Point* data,
	const int data_arr_max,
	const Centroid* centroid,
	const int n_cluster)
{
	int i, j, k;
	float min = MAX_NUM_FOR_MIN;
	int numbering = 0;														// ���° centroid ����
	float* centroid_clustering = (float*)calloc(n_cluster, sizeof(float));	// ��Ŭ���� �Ÿ� ��� �� �����ϱ�

	if (centroid_clustering == NULL) return;

	for (i = 0; i < data_arr_max; i++)
	{
		// �� �������� ��ġ�� centroid�� ��ġ�� ��Ŭ���� �Ÿ��� ���
		for (j = 0; j < n_cluster; j++)
			centroid_clustering[j] = euclideanDistance(data[i], centroid[j]);

		for (j = 0; j < n_cluster; j++)
		{
			if (centroid_clustering[j] < min)
			{
				min = centroid_clustering[j];
				numbering = j;
			}
		}
		
		min = MAX_NUM_FOR_MIN;
		data[i].centroid_num = numbering;
	}

	free(centroid_clustering);
}

// ��Ŭ����� �Ÿ� ���
float euclideanDistance(Point data, Centroid centroid)
{
	return sqrt(pow(data.x - centroid.x, 2) + pow(data.y - centroid.y, 2));
}

// �߽����� ��ġ ����
void toCentroidCenter(
	Point* data,
	const int data_arr_max,
	Centroid* centroid,
	const int n_cluster)
{
	Point* centroid_to_center = (Point*)calloc(n_cluster, sizeof(Point));
	int i;

	if (centroid_to_center == NULL) return -1;

	for (i = 0; i < data_arr_max; i++)
	{
		centroid_to_center[data[i].centroid_num].x += data[i].x;
		centroid_to_center[data[i].centroid_num].y += data[i].y;
		centroid_to_center[data[i].centroid_num].centroid_num++;
	}

	// ��� ���
	for (i = 0; i < n_cluster; i++)
	{
		centroid[i].x = centroid_to_center[i].x / centroid_to_center[i].centroid_num;
		centroid[i].y = centroid_to_center[i].y / centroid_to_center[i].centroid_num;
	}

	free(centroid_to_center);
}

// Centroid�� ���� �����Ͽ� �� ���� ��
Centroid* centroid_copy(Centroid* centroid, const int n_cluster)
{
	Centroid* copy = (Centroid*)malloc(sizeof(Centroid) * n_cluster);
	int i;

	if (copy == NULL) return -1;

	for (i = 0; i < n_cluster; i++)
	{
		copy[i].x = centroid[i].x;
		copy[i].y = centroid[i].y;
	}

	return copy;
}

// Centroid�� ���� �ݺ��ϱ� ���� ���� ������ �� Ȯ��
// 1: ��ġ ���̹Ƿ� k-means ����
// 0: ����ġ ���̹Ƿ� cluster �ٽ� ����
int checkDoesCentroidIsSameWithBefore(
	Centroid* centroid,
	Centroid* centroid_before,
	const int n_cluster)
{
	int i, check = 0;

	for (i = 0; i < n_cluster; i++)
	{
		if (centroid[i].x == centroid_before[i].x && centroid[i].y == centroid_before[i].y)
			check = 1;
	}
	return check;
}

// ��� ���
void result(const Point* data, const int data_arr_max, const Centroid* centroid, const int n_cluster)
{
	int* sum = (int*)calloc(n_cluster, sizeof(int));
	int i;

	if (sum == NULL) return -1;

	//�� ������ ��ü��
	for (i = 0; i < data_arr_max; i++)
		sum[data[i].centroid_num]++;

	for (i = 0; i < n_cluster; i++)
		printf("���� ��ȣ: %d. ��ü��: %d\n", i + 1, sum[i]);

	free(sum);
}