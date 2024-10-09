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

#define MAX_BUFFER 1024							// 읽을 최대 줄
#define TERMINAL_MAX_SIZE 45

void selectXY(const char* str_tmp);

int main(const int argc, const char* argv[])
{
	FILE* csvOpen = NULL;
	Point *data;								// 포인트 데이터 (동적할당)
	char str_tmp[MAX_BUFFER];					// 한 행의 최대 길이
	int selectX, selectY;						// selectX = sepal length, selectY = sepal Width
	int data_max_index = 0, i, index;			// data_max_index = 배열 data_max_index의 최대값
	int n_clusters = 1;							// 군집의 개수
	Centroid* centroid, * centroid_before;		// 랜덤의 위치 생성할 Centroid, 복사할 Centroid
	int refresh_times = 0;						// 갱신 횟수
	int temp = 0;								// 중심점(centroid) 갯수
	clock_t start, end;							// 소요시간 계산

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
	
	data_max_index = checkMaxIndex(csvOpen);
	data = (Point*)calloc(data_max_index + 1, sizeof(Point));
	fopenMalloc(csvOpen, data, argv[1], selectX, selectY);

	centroid = (Centroid*)calloc(n_clusters, sizeof(Centroid));
	start = clock();						// 총 소요시간 계산 시작

	// k-means++
	// 무작위 선택되는 첫번째 중심점
	temp = rand() % data_max_index;
	centroid[0].x = data[temp].x;
	centroid[0].y = data[temp].y;

	// 새로운 K를 찾기 위한 반복
	while (1) {
		if (!(index = kmeans_pp_centroidnum(data, data_max_index, centroid, n_clusters)))
			break;
		n_clusters++;
		centroid = realloc(centroid, sizeof(Centroid) * n_clusters);
		centroid[n_clusters - 1].x = data[index].x;
		centroid[n_clusters - 1].y = data[index].y;
	}

	printf("n_cluster: %d\n", n_clusters);
	// Centroid의 위치가 전 위치에 동일될 때까지 반복
	while (1) {
		centroid_before = centroid_copy(centroid, n_clusters);
		clustering(data, data_max_index, centroid, n_clusters);
		toCentroidCenter(data, data_max_index, centroid, n_clusters);
		refresh_times++;
		if (checkDoesCentroidIsSameWithBefore(centroid, centroid_before, n_clusters))
			break;
		free(centroid_before);
	}

	result(data, data_max_index, centroid, n_clusters, data_max_index);
	end = clock();							// 총 소요시간 계산 종료
	printf("소요시간: %f 밀리초[ms]\n", (double)(end - start) / CLOCKS_PER_SEC);

	if (data_max_index <= 1500)
	{
		// 포인터를 터미널에 그리기
		printf("데이터를 터미널에 그릴까요? (1을 입력 / 0으로 종료): ");
		scanf("%d", &temp);
		if (temp)
			figurePointing(argv[1], data, data_max_index, n_clusters);
	}
	else
		printf("데이터가 너무 많아 터미널에 모두 그릴 수가 없습니다.\n");

	free(data);
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