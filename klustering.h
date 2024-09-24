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

void dataToArray(const char* str_tmp, const int x, const int y, Point* data, const int index);		// CSV의 모든 데이터들을 Point 배열로 넣기
void clustering(Point* data, const int data_arr_max, const Centroid* centroid, const int n_cluster);
float euclideanDistance(Point data, Centroid centroid);
void toCentroidCenter(Point* data, const int data_arr_max, Centroid* centroid, const int n_cluster);

#endif // !__KLUSTERING_H__
