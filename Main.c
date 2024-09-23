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

#include <stdio.h>

#include "klustering.h"

#define MAX_BUFFER 1024
#define TERMINAL_MAX_SIZE 45

int main(const int argc, const char* argv[])
{
	FILE* csvOpen = NULL;
	Point data[MAX_BUFFER] = { 0.0, 0.0, 0 };
	char str_tmp[MAX_BUFFER];
	int selectX = 0, selectY = 1;		// selectX = sepal length, selectY = sepal Width
	int n_clusters = 0, i;					// 군집의 개수
	int max_index = 0;

	if (fopen_s(&csvOpen, argv[1], "r") != NULL)
	{
		printf("파일을 못 찾았습니다.");
		return -1;
	}

	fgets(str_tmp, MAX_BUFFER, csvOpen);
	while (!feof(csvOpen))
	{
		fgets(str_tmp, MAX_BUFFER, csvOpen);
		dataToArray(str_tmp, selectX, selectY, max_index);
		max_index++;
	}


	fclose(csvOpen);
	return 0;
}