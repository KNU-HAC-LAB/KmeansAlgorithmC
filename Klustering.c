#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "klustering.h"

#define MAX(x,y) (((x)>(y))?(x):(y))

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

// ��Ŭ����� �Ÿ� ���
float euclideanDistance_ForAnotherData(Point data, Point choosen_data)
{
	return sqrt(pow(data.x - choosen_data.x, 2) + pow(data.y - choosen_data.y, 2));
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

	if (copy == NULL) return;

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

// �� �����Ϳ� ���� �� Ŭ�������� �Ƿ翧 ���
float* silhouette_Coefficient(
	int index, const Point* data, const int data_arr_max, const int n_cluster)
{
	Point chooseone;
	int i;
	float* cluster_silhoutte = (float*)calloc(n_cluster, sizeof(float));

	chooseone.x = data[index].x;
	chooseone.y = data[index].y;
	chooseone.centroid_num = data[index].centroid_num;

	for (i = 0; i < n_cluster; i++)
	{
		if (chooseone.centroid_num == i)
			cluster_silhoutte[i] = cluster_avg(1, chooseone, data, data_arr_max);
		else
			cluster_silhoutte[i] = cluster_avg(0, chooseone, data, data_arr_max);
	}

	return cluster_silhoutte;
}

// �����Ϳ� Ŭ������ ������ �Ÿ� ���
// same_cluster: ���� cluster �ȿ� �ִ� ��?
float cluster_avg(int same_cluster, const Point choosen, const Point* data, const int data_arr_max)
{
	int i, max = 0;
	float avg = 0.0;

	for (i = 0; i < data_arr_max; i++)
	{
		if (choosen.centroid_num != data[i].centroid_num)
			continue;
		avg += euclideanDistance_ForAnotherData(data[i], choosen);
		max++;
	}

	if (same_cluster) max--;

	avg /= max;
	return avg;
}

// ��� ���
void result(
	const Point* data,
	const int data_arr_max,
	const Centroid* centroid,
	const int n_cluster,
	const int max_index)
{
	int i, j, chooseone_sum = 0, theMaxCluster = 0;
	int *sum = (int*)calloc(n_cluster, sizeof(int));
	float* cluster_avg, max = MAX_NUM_FOR_MIN, avg = 0.0;
	float* pointSilhouette = (float*)calloc(data_arr_max, sizeof(float));

	// �Ƿ翧 ��� a(i)
	for (i = 0; i < data_arr_max; i++)
	{
		sum[data[i].centroid_num]++;	// �� ������ ��ü��
		cluster_avg = silhouette_Coefficient(i, data, data_arr_max, n_cluster);

		for (j = 0; j < n_cluster; j++)
		{
			if (data[i].centroid_num == j)
				continue;

			if (cluster_avg[j] < max)
				max = cluster_avg[j];
		}

		pointSilhouette[i] = silhouette_calculate(cluster_avg[data[i].centroid_num], max);
		/*printf("cluster_avg[%d]: %f, pointSilhouette[%d]: %f\n",
			data[i].centroid_num, cluster_avg[data[i].centroid_num], i, pointSilhouette[i]);*/
		avg += pointSilhouette[i];
		free(cluster_avg);
	}
	printf("avg: %f\n", avg);

	avg /= max_index;

	for (i = 0; i < n_cluster; i++)
		printf("���� ��ȣ: %d. ��ü��: %d\n", i + 1, sum[i]);
	printf("�Ƿ翧 �����: %f\n", avg);

	free(pointSilhouette);
	free(sum);
}

// ����
float silhouette_calculate(const float first_silhoutte, const float second_silhoutte)
{
	return (first_silhoutte - second_silhoutte) / MAX(first_silhoutte, second_silhoutte);
}