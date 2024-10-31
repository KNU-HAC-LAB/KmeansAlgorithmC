/*
 * 노홍림 202205088
 * 주제: C언어로 Kmeans 알고리즘 재현
 * 
 * Main 파일
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "clustering.h"

#define MAX_BUFFER 1024							// 읽을 최대 줄

void selectXY(const char* str_tmp);

int main(const int argc, const char* argv[])
{
	FILE* csvOpen = NULL;
	Point* data, * original;					// 포인트 데이터 (동적할당)
	char str_tmp[MAX_BUFFER];					// 한 행의 최대 길이
	int selectX, selectY;						// selectX = sepal length, selectY = sepal Width
	int data_max_index = 0, index;				// data_max_index = 배열 data_max_index의 최대값
	Centroid* centroid, * centroid_before;		// 랜덤의 위치 생성할 Centroid, 복사할 Centroid
	int n_clusters = 1;							// 군집의 개수
	clock_t start, end;							// 소요시간 계산
	int temp = 0;

	//int l, k;									// Scalable K-means++를 위한 변수

	srand((unsigned int)time(NULL));			// seed값 고정

	if (fopen_s(&csvOpen, argv[1], "r") != NULL)
	{
		printf("파일을 못 찾았습니다.");
		return -1;
	}

	fgets(str_tmp, MAX_BUFFER, csvOpen);
	selectXY(str_tmp);
	printf("X축에 사용할 데이터 선택: ");
	scanf("%d", &selectX);
	printf("Y축에 사용할 데이터 선택: ");
	scanf("%d", &selectY);

	selectX--;
	selectY--;

	data_max_index = total(csvOpen);
	data = (Point*)calloc(data_max_index + 1, sizeof(Point));
	fopenMalloc(csvOpen, data, argv[1], selectX, selectY);
	centroid = (Centroid*)calloc(n_clusters, sizeof(Centroid));

	start = clock();						// 총 소요시간 계산 시작
	original = (Point*)calloc(data_max_index + 1, sizeof(Point));
	data_represent(data, original, data_max_index);

	// k-means++
	// 무작위 선택되는 첫번째 중심점
	temp = rand() % data_max_index;
	centroid[0].x = data[temp].x;
	centroid[0].y = data[temp].y;

	// 새로운 Centroid를 찾기 위한 반복
	while (1) {
		if (!(index = kmeans_pp_centroidnum(data, data_max_index, centroid, n_clusters)))
			break;
		centroid = (Centroid*)realloc(centroid, sizeof(Centroid) * ++n_clusters);
		centroid[n_clusters - 1].x = data[index].x;
		centroid[n_clusters - 1].y = data[index].y;
	}

	printf("전체 데이터 수: %d\n", data_max_index);
	//printf("희망 clusters 수(k): \n");

	// Centroid의 위치가 전 위치에 동일될 때까지 반복
	while (1) {
		centroid_before = centroid_copy(centroid, n_clusters);
		clustering(data, data_max_index, centroid, n_clusters);
		toCentroidCenter(data, data_max_index, centroid, n_clusters);
		if (checkDoesCentroidIsSameWithBefore(centroid, centroid_before, n_clusters))
			break;
		free(centroid_before);
	}

	result(data, original, data_max_index, centroid, n_clusters, data_max_index);

	for (int i = 0; i < 20; i++)
		printf("data[%d].x: %f, data[%d].y: %f, data[%d].centroid: %d\n", i, data[i].x, i, data[i].y, i, data[i].centroid_num);
	end = clock();							// 총 소요시간 계산 종료
	printf("소요시간: %f 밀리초[ms]\n", (double)(end - start) / CLOCKS_PER_SEC);
	free(data);
	free(centroid_before);
	free(centroid);
	free(original);
	fclose(csvOpen);
	return 0;
}

void selectXY(const char* str_tmp)
{
	char* token = strtok(str_tmp, ",");
	int i = 0;

	while (token != NULL)
	{
		i++;
		printf("%d. %s\n", i, token);
		token = strtok(NULL, ",");
	}
}