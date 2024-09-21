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
#include <stdlib.h>

#include "terminal.h"
#include "graphLocation.h"

#define MAX_BUFFER 1024

int main(const int argc, const char* argv[])
{
	FILE* csvOpen = NULL;
	char str_tmp[MAX_BUFFER];
	//int select;

	if (fopen_s(&csvOpen, argv[1], "r") != NULL)
	{
		printf("파일을 못 찾았습니다.");
		return -1;
	}
	fgets(str_tmp, MAX_BUFFER, csvOpen);
	printf("k-means에 사용될 x축:\n");
	selectTheXYData(str_tmp);
	//cordinatesInit(50);

	while (!feof(csvOpen))
	{
		fgets(str_tmp, MAX_BUFFER, csvOpen);
		//printf("%s", str_tmp);
	}

	fclose(csvOpen);
	return 0;
}