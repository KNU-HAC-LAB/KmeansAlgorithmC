/*
 * 노홍림 202205088
 * 주제: C언어로 Kmeans 알고리즘 재현
 * 
 * Main 파일
 */

/*
순서: 
1. K-means를 위한 CSV데이터 받기	Main.c
2. 데이터를 gotoxy로 시각화	Terminal.c
3. 클러스터링	
4. 
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "klustering.h"

#define MAX_BUFFER 1024
#define TERMINAL_MAX_SIZE 45

int main(const int argc, const char* argv[])
{
	FILE* csvOpen = NULL;
	Point data[MAX_BUFFER] = { 0.0, 0.0, 0 };
	char str_tmp[MAX_BUFFER];
	const int selectX = 0, selectY = 1;			// selectX = sepal length, selectY = sepal Width
	int max_index = 0, i;						// max_index: 배열 max_index의 최대값
	int n_clusters;								// 군집의 개수
	Centroid* centroid;							// 랜덤의 위치에서 생성된
	int temp = 0;

	srand((unsigned int)time(NULL));

	if (fopen_s(&csvOpen, argv[1], "r") != NULL)
	{
		printf("파일을 못 찾았습니다.");
		return -1;
	}

	fgets(str_tmp, MAX_BUFFER, csvOpen);
	while (!feof(csvOpen))
	{
		fgets(str_tmp, MAX_BUFFER, csvOpen);
		dataToArray(str_tmp, selectX, selectY, data, max_index);
		max_index++;
	}

	printf("n_clusters: ");
	scanf("%d", &n_clusters);

	centroid = (Centroid*)calloc(n_clusters, sizeof(Centroid));

	if (centroid == NULL) return -1;

	for (i = 0; i < n_clusters; i++)
	{
		temp = rand() % max_index;
		centroid[i].x = data[temp].x;
		centroid[i].y = data[temp].y;
		printf("i: %d, centroid[i].x: %f, centroid[i].y: %f\n", i, centroid[i].x, centroid[i].y);
	}
	
	clustering(data, max_index, centroid, n_clusters);

	printf("Centroid_num: \n\n");
	for (i = 0; i < max_index; i++)
		printf("i: %d, %d\n", i, data[i].centroid_num);

	free(centroid);
	fclose(csvOpen);
	return 0;
}