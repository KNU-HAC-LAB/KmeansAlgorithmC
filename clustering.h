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

// CSV�� �ִ� �����͸� �迭�� �ű��
int checkMaxIndex(const FILE* csvOpen);
void fopenMalloc(const FILE* csvOpen, Point* data, const char* argv, const int selectX, const int selectY);
void dataToArray(const char* str_tmp, const int x, const int y, Point* data, const int index);

// Ŭ�����͸� �ڵ�
void clustering(Point* data, const int data_arr_max, const Centroid* centroid, const int n_cluster);

// ��Ŭ���� �Ÿ� ���
float euclideanDistance(Point data, Centroid centroid);
float euclideanDistance_ForAnotherData(Point data, Point choosen_data);

// �߽����� ��ġ ����
void toCentroidCenter(Point* data, const int data_arr_max, Centroid* centroid, const int n_cluster);

// Centroid ���� ����
Centroid* centroid_copy(Centroid* centroid, const int n_cluster);

// Centroid ���� �ݺ� �� ���� ���� ������ �� Ȯ��
int checkDoesCentroidIsSameWithBefore(Centroid* centroid, Centroid* centroid_before, const int n_cluster);

// Ŭ������ �� �� �������� �Ƿ翧 ���
float* silhouette_Coefficient(int index, const Point* data, const int data_arr_max, const int n_cluster);
float cluster_avg(int same_cluster, const Point choosen, const Point* data, const int data_arr_max);
float silhouette_calculate(const float first_silhoutte, const float second_silhoutte);

// ��� ���
void result(const Point* data, const int data_arr_max, const Centroid* centroid, const int n_cluster, const int max_index);

#endif // !__KLUSTERING_H__
