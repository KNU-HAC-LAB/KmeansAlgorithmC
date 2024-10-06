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
#include "terminal.h"
#include "graphLocation.h"

#define MAX_BUFFER 1024
#define TERMINAL_MAX_SIZE 45

void selectXY(const char* str_tmp);

int main(const int argc, const char* argv[])
{
	FILE* csvOpen = NULL;
	Point data[MAX_BUFFER] = { 0.0, 0.0, 0 };
	char str_tmp[MAX_BUFFER];
	int selectX, selectY;						// selectX = sepal length, selectY = sepal Width
	int max_index = 0, i;						// max_index: 배열 max_index의 최대값
	int n_clusters;								// 군집의 개수
	Centroid* centroid, * centroid_before;		// 랜덤의 위치 생성할 Centroid, 복사할 Centroid
	int refresh_times = 0;						// 갱신 횟수
	int temp = 0;
	clock_t start, end;

	//srand((unsigned int)time(NULL));			// seed 고정

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

	while (!feof(csvOpen))
	{
		fgets(str_tmp, MAX_BUFFER, csvOpen);
		dataToArray(str_tmp, selectX, selectY, data, max_index);
		max_index++;
	}

	printf("n_clusters: ");
	scanf("%d", &n_clusters);

	start = clock();
	centroid = (Centroid*)calloc(n_clusters, sizeof(Centroid));

	if (centroid == NULL) return -1;

	for (i = 0; i < n_clusters; i++)		// 무작위 데이터 중의 하나로 시작 점을 설정
	{
		temp = rand() % max_index;
		centroid[i].x = data[temp].x;
		centroid[i].y = data[temp].y;
	}

	// Centroid의 위치가 전 위치에 동일될 때까지 반복
	while (1) {
		centroid_before = centroid_copy(centroid, n_clusters);
		clustering(data, max_index, centroid, n_clusters);
		toCentroidCenter(data, max_index, centroid, n_clusters);
		refresh_times++;
		if (checkDoesCentroidIsSameWithBefore(centroid, centroid_before, n_clusters))
			break;
		free(centroid_before);
	}

	result(data, max_index, centroid, n_clusters, max_index);
	end = clock();
	printf("소요시간: %f 밀리초[ms]\n", (double)(end - start) / CLOCKS_PER_SEC);

	// 포인터를 터미널에 그리기
	printf("데이터를 터미널에 그릴까요? (1을 입력 / 0으로 종료): ");
	scanf("%d", &temp);
	if (temp)
		figurePointing(argv[1], data, max_index, n_clusters);

	free(centroid_before);
	free(centroid);
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