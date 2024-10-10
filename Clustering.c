#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "clustering.h"

#define MAX(x,y) (((x)>(y))?(x):(y))

#define MAX_NUM_FOR_MIN 100
#define MAX_BUFFER (1024)

// stackoverflow�� �����ϱ� ���� data�� �����Ҵ� �ϱ�
int total(FILE * csvOpen)
{
	int max_index = 0;
	char str_tmp[MAX_BUFFER];
	fgets(str_tmp, MAX_BUFFER, csvOpen);

	while (!feof(csvOpen))
	{
		fgets(str_tmp, MAX_BUFFER, csvOpen);
		max_index++;
	}
	return max_index;
}

void fopenMalloc(const FILE* csvOpen, Point* data, const char* argv, const int selectX, const int selectY)
{
	int max_index = 0;
	char str_tmp[MAX_BUFFER];

	fopen_s(&csvOpen, argv, "r");
	fgets(str_tmp, MAX_BUFFER, csvOpen);

	while (!feof(csvOpen))
	{
		fgets(str_tmp, MAX_BUFFER, csvOpen);
		dataToArray(str_tmp, selectX, selectY, data, max_index);
		max_index++;
	}
}

// CSV�� �ִ� �����͸� �迭�� �ű��
void dataToArray(
	const char* str_tmp,
	const int x,
	const int y,
	Point* data,
	const int index)
{
	char* token = strtok(str_tmp, ",");
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
}

int kmeans_pp_centroidnum(const Point* data, const int max_index, const Centroid* centroid, const int n_clusters)
{
	float big_num = 0;
	float* every_distance = (float*)calloc(max_index, sizeof(float));
	float* px = (float*)calloc(max_index, sizeof(float));
	float sum = 0.0;
	int i, index = 0;

	// ��� �������� ��Ŭ���� �Ÿ� ��� D(x)
	for (i = 0; i < max_index; i++)
		every_distance[i] = euclideanDistance(data[i], centroid[n_clusters - 1]);

	// D(x)^2 �� ���
	for (i = 0; i < max_index; i++)
		every_distance[i] = pow(every_distance[i], 2);

	// p(x) �� ���
	sum = px_sum(every_distance, max_index);
	for (i = 0; i < max_index; i++)
		px[i] = every_distance[i] / sum;
	free(every_distance);

	for (i = 0; i < max_index; i++)
	{
		if (px[i] > big_num)		// ���� ū p(x) ���� ���� �߽��� (Centroid)�� �ȴ�.
		{
			big_num = px[i];
			index = i;
		}
	}
	free(px);
	
	/*printf("big_num: %f, n_clusters - 1: %d, centroid[n_clusters - 1].x: %.f, centroid[n_clusters - 1].y: %.f, index: %d, data[index].x: %.f, data[index].y: %.f\n", 
		big_num, n_clusters - 1, centroid[n_clusters - 1].x, centroid[n_clusters - 1].y, index, data[index].x, data[index].y);*/
	
	// ���� centroid ��ġ�� �����ϸ� ����, �ƴϸ� ��� �ݺ��Ͽ� centroid�� �����
	for (i = 0; i < n_clusters; i++)
	{
		if (centroid[i].x == data[index].x && centroid[i].y == data[index].y)
			return 0;
	}
	return index;
}

// sum of p(x)
float px_sum(const float* every_dx_square, const int max_index)
{
	float sum = 0.0;
	int i;
	for (i = 0; i < max_index; i++)
		sum += every_dx_square[i];
	return sum;
}

// Ŭ�����͸� �ڵ�
void clustering(
	Point* data,
	const int data_arr_max,
	const Centroid* centroid,
	const int n_clusters)
{
	int i, j;
	float min = MAX_NUM_FOR_MIN;
	int numbering = 0;														// ���° centroid ����
	float* centroid_clustering = (float*)calloc(n_clusters, sizeof(float));	// ��Ŭ���� �Ÿ� ��� �� �����ϱ�

	for (i = 0; i < data_arr_max; i++)
	{
		// �� �������� ��ġ�� centroid�� ��ġ�� ��Ŭ���� �Ÿ��� ���
		for (j = 0; j < n_clusters; j++)
			centroid_clustering[j] = euclideanDistance(data[i], centroid[j]);

		for (j = 0; j < n_clusters; j++)
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
	const int n_clusters)
{
	Point* centroid_to_center = (Point*)calloc(n_clusters, sizeof(Point));
	int i;

	if (centroid_to_center == NULL) return -1;

	for (i = 0; i < data_arr_max; i++)
	{
		centroid_to_center[data[i].centroid_num].x += data[i].x;
		centroid_to_center[data[i].centroid_num].y += data[i].y;
		centroid_to_center[data[i].centroid_num].centroid_num++;
	}

	// ��� ���
	for (i = 0; i < n_clusters; i++)
	{
		centroid[i].x = centroid_to_center[i].x / centroid_to_center[i].centroid_num;
		centroid[i].y = centroid_to_center[i].y / centroid_to_center[i].centroid_num;
	}

	free(centroid_to_center);
}

// Centroid�� ���� �����Ͽ� �� ���� ��
Centroid* centroid_copy(Centroid* centroid, const int n_clusters)
{
	Centroid* copy = (Centroid*)malloc(sizeof(Centroid) * n_clusters);
	int i;

	if (copy == NULL) return;

	for (i = 0; i < n_clusters; i++)
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
	const int n_clusters)
{
	int i, check = 0;

	for (i = 0; i < n_clusters; i++)
	{
		if (centroid[i].x == centroid_before[i].x && centroid[i].y == centroid_before[i].y)
			check = 1;
	}
	return check;
}

// ���� �����Ϳ� ���� �Ƿ翧 ���
float* silhouette_Coefficient(
	int index, const Point* data, const int data_arr_max, const int n_clusters)
{
	Point chooseone;
	int i;
	float* cluster_silhoutte = (float*)calloc(n_clusters, sizeof(float));

	chooseone.x = data[index].x;
	chooseone.y = data[index].y;
	chooseone.centroid_num = data[index].centroid_num;

	for (i = 0; i < n_clusters; i++)
	{
		// �� �����Ϳ� ���ԵǾ� �ִ� Ŭ�����Ϳ��� �ϳ� ����
		if (chooseone.centroid_num == i)
			cluster_silhoutte[i] = cluster_avg(1, chooseone, data, data_arr_max);
		else
			cluster_silhoutte[i] = cluster_avg(0, chooseone, data, data_arr_max);
	}

	return cluster_silhoutte;
}

// ���õ� �����Ϳ� �ٸ� ������ ������ �Ÿ� ���
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

// Min-Max ����ȭ ����
float min_max_represent(const float max_data, const float min_data, const float this_data)
{
	return (this_data - min_data) / (max_data - min_data);
}
// ����ȭ �����͸� ���� �����Ϳ� �ֱ�
void data_represent(Point* data, Point* original, const int data_arr_max, Centroid* max_data, Centroid* min_data)
{
	int i;

	for (i = 0; i < data_arr_max; i++)
	{
		original[i].x = data[i].x;
		original[i].y = data[i].y;

		if (data[i].x > max_data->x)
			max_data->x = data[i].x;
		if (data[i].y > max_data->y)
			max_data->y = data[i].y;

		if (data[i].x < min_data->x)
			min_data->x = data[i].x;
		if (data[i].y < min_data->y)
			min_data->y = data[i].y;
	}

	for (i = 0; i < data_arr_max; i++)
	{
		data[i].x = min_max_represent(max_data->x, min_data->x, data[i].x);
		data[i].y = min_max_represent(max_data->y, min_data->y, data[i].y);
	}
}

// ��� ���
void result(
	const Point* data,
	Point* original,
	const int data_arr_max,
	const Centroid* centroid,
	const int n_clusters,
	const int max_index
){
	int i, j, chooseone_sum = 0, theMaxCluster = 0;
	int *sum = (int*)calloc(n_clusters, sizeof(int));
	float* cluster_avg, max = MAX_NUM_FOR_MIN, avg = 0.0;
	float* pointSilhouette = (float*)calloc(data_arr_max, sizeof(float));

	// �Ƿ翧 ��� a(i)
	for (i = 0; i < data_arr_max; i++)
	{
		sum[data[i].centroid_num]++;	// �� ������ ��ü��
		original[i].centroid_num = data[i].centroid_num;
		cluster_avg = silhouette_Coefficient(i, data, data_arr_max, n_clusters);

		for (j = 0; j < n_clusters; j++)
		{
			if (data[i].centroid_num == j)
				continue;

			if (cluster_avg[j] < max)
				max = cluster_avg[j];
		}

		pointSilhouette[i] = silhouette_calculate(cluster_avg[data[i].centroid_num], max);
		avg += pointSilhouette[i];
		free(cluster_avg);
	}

	avg /= max_index;

	for (i = 0; i < n_clusters; i++)
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