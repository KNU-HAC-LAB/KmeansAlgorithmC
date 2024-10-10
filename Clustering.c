#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "clustering.h"

#define MAX(x,y) (((x)>(y))?(x):(y))

#define MAX_NUM_FOR_MIN 100
#define MAX_BUFFER (1024)

// stackoverflow를 방지하기 위해 data를 동적할당 하기
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

// CSV에 있는 데이터를 배열로 옮기기
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

	// 모든 데이터의 유클리드 거리 계산 D(x)
	for (i = 0; i < max_index; i++)
		every_distance[i] = euclideanDistance(data[i], centroid[n_clusters - 1]);

	// D(x)^2 값 계산
	for (i = 0; i < max_index; i++)
		every_distance[i] = pow(every_distance[i], 2);

	// p(x) 값 계산
	sum = px_sum(every_distance, max_index);
	for (i = 0; i < max_index; i++)
		px[i] = every_distance[i] / sum;
	free(every_distance);

	for (i = 0; i < max_index; i++)
	{
		if (px[i] > big_num)		// 제일 큰 p(x) 값이 다음 중심점 (Centroid)이 된다.
		{
			big_num = px[i];
			index = i;
		}
	}
	free(px);
	
	/*printf("big_num: %f, n_clusters - 1: %d, centroid[n_clusters - 1].x: %.f, centroid[n_clusters - 1].y: %.f, index: %d, data[index].x: %.f, data[index].y: %.f\n", 
		big_num, n_clusters - 1, centroid[n_clusters - 1].x, centroid[n_clusters - 1].y, index, data[index].x, data[index].y);*/
	
	// 전의 centroid 위치와 동일하면 종료, 아니면 계속 반복하여 centroid를 만들기
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

// 클러스터링 코드
void clustering(
	Point* data,
	const int data_arr_max,
	const Centroid* centroid,
	const int n_clusters)
{
	int i, j;
	float min = MAX_NUM_FOR_MIN;
	int numbering = 0;														// 몇번째 centroid 인지
	float* centroid_clustering = (float*)calloc(n_clusters, sizeof(float));	// 유클리드 거리 계산 후 저장하기

	for (i = 0; i < data_arr_max; i++)
	{
		// 각 데이터의 위치와 centroid의 위치를 유클리드 거리로 계산
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

// 유클리디안 거리 계산
float euclideanDistance(Point data, Centroid centroid)
{
	return sqrt(pow(data.x - centroid.x, 2) + pow(data.y - centroid.y, 2));
}

// 유클리디안 거리 계산
float euclideanDistance_ForAnotherData(Point data, Point choosen_data)
{
	return sqrt(pow(data.x - choosen_data.x, 2) + pow(data.y - choosen_data.y, 2));
}

// 중심으로 위치 갱신
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

	// 평균 계산
	for (i = 0; i < n_clusters; i++)
	{
		centroid[i].x = centroid_to_center[i].x / centroid_to_center[i].centroid_num;
		centroid[i].y = centroid_to_center[i].y / centroid_to_center[i].centroid_num;
	}

	free(centroid_to_center);
}

// Centroid를 깊은 복사하여 전 값과 비교
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

// Centroid의 값이 반복하기 전의 값과 동일한 지 확인
// 1: 일치 값이므로 k-means 종료
// 0: 비일치 값이므로 cluster 다시 실행
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

// 각각 데이터에 대한 실루엣 계수
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
		// 이 데이터에 포함되어 있는 클러스터에서 하나 제외
		if (chooseone.centroid_num == i)
			cluster_silhoutte[i] = cluster_avg(1, chooseone, data, data_arr_max);
		else
			cluster_silhoutte[i] = cluster_avg(0, chooseone, data, data_arr_max);
	}

	return cluster_silhoutte;
}

// 선택된 데이터와 다른 데이터 사이의 거리 평균
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

// Min-Max 정규화 공식
float min_max_represent(const float max_data, const float min_data, const float this_data)
{
	return (this_data - min_data) / (max_data - min_data);
}
// 정규화 데이터를 기존 데이터에 넣기
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

// 결과 출력
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

	// 실루엣 계수 a(i)
	for (i = 0; i < data_arr_max; i++)
	{
		sum[data[i].centroid_num]++;	// 각 군집의 개체수
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
		printf("군집 번호: %d. 개체수: %d\n", i + 1, sum[i]);
	printf("실루엣 계수값: %f\n", avg);

	free(pointSilhouette);
	free(sum);
}

// 공식
float silhouette_calculate(const float first_silhoutte, const float second_silhoutte)
{
	return (first_silhoutte - second_silhoutte) / MAX(first_silhoutte, second_silhoutte);
}