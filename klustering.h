#ifndef __KLUSTERING_H__
#define __KLUSTERING_H__

typedef struct {
	float x;
	float y;
	int centroid_num;
} Point;

typedef struct {
	float x;
	float y;
} Centroid;

// CSV에 있는 데이터를 배열로 옮기기
void dataToArray(const char* str_tmp, const int x, const int y, Point* data, const int index);

// 클러스터링 코드
void clustering(Point* data, const int data_arr_max, const Centroid* centroid, const int n_cluster);

// 유클리드 거리 계산
float euclideanDistance(Point data, Centroid centroid);

// 중심으로 위치 갱신
void toCentroidCenter(Point* data, const int data_arr_max, Centroid* centroid, const int n_cluster);

// Centroid 깊은 복사
Centroid* centroid_copy(Centroid* centroid, const int n_cluster);

// Centroid 값이 반복 전 과의 값이 동일한 지 확인
int checkDoesCentroidIsSameWithBefore(Centroid* centroid, Centroid* centroid_before, const int n_cluster);

#endif // !__KLUSTERING_H__
