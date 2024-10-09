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
int checkMaxIndex(const FILE* csvOpen);
void fopenMalloc(const FILE* csvOpen, Point* data, const char* argv, const int selectX, const int selectY);
void dataToArray(const char* str_tmp, const int x, const int y, Point* data, const int index);

// 클러스터링 코드
void clustering(Point* data, const int data_arr_max, const Centroid* centroid, const int n_cluster);

// 유클리드 거리 계산
float euclideanDistance(Point data, Centroid centroid);
float euclideanDistance_ForAnotherData(Point data, Point choosen_data);

// 중심으로 위치 갱신
void toCentroidCenter(Point* data, const int data_arr_max, Centroid* centroid, const int n_cluster);

// Centroid 깊은 복사
Centroid* centroid_copy(Centroid* centroid, const int n_cluster);

// Centroid 값이 반복 전 과의 값이 동일한 지 확인
int checkDoesCentroidIsSameWithBefore(Centroid* centroid, Centroid* centroid_before, const int n_cluster);

// 클러스터 후 한 데이터의 실루엣 계수
float* silhouette_Coefficient(int index, const Point* data, const int data_arr_max, const int n_cluster);
float cluster_avg(int same_cluster, const Point choosen, const Point* data, const int data_arr_max);
float silhouette_calculate(const float first_silhoutte, const float second_silhoutte);

// 결과 출력
void result(const Point* data, const int data_arr_max, const Centroid* centroid, const int n_cluster, const int max_index);

#endif // !__KLUSTERING_H__
