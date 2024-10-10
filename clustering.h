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
int total(FILE* csvOpen);
void fopenMalloc(FILE* csvOpen, Point* data, const char* argv, const int selectX, const int selectY);
void dataToArray(char* str_tmp, const int x, const int y, Point* data, const int index);

// kmeans++
int kmeans_pp_centroidnum(const Point* data, const int max_index, const Centroid* centroid, const int n_clusters);
float px_sum(const float* every_dx_square, const int max_index);

// ����ȭ
float min_max_represent(const float max_data, const float min_data, const float this_data);
void data_represent(Point* data, Point* original, const int data_arr_max);

// Ŭ�����͸� �ڵ�
void clustering(Point* data, const int data_arr_max, const Centroid* centroid, const int n_clusters);

// ��Ŭ���� �Ÿ� ���
float euclideanDistance(Point data, Centroid centroid);
float euclideanDistance_ForAnotherData(Point data, Point choosen_data);

// �߽����� ��ġ ����
void toCentroidCenter(Point* data, const int data_arr_max, Centroid* centroid, const int n_clusters);

// Centroid ���� ����
Centroid* centroid_copy(Centroid* centroid, const int n_clusters);

// Centroid ���� �ݺ� �� ���� ���� ������ �� Ȯ��
int checkDoesCentroidIsSameWithBefore(Centroid* centroid, Centroid* centroid_before, const int n_clusters);

// Ŭ������ �� �� �������� �Ƿ翧 ���
float* silhouette_Coefficient(int index, const Point* data, const int data_arr_max, const int n_clusters);
float cluster_avg(int same_cluster, const Point choosen, const Point* data, const int data_arr_max);
float silhouette_calculate(const float first_silhoutte, const float second_silhoutte);

// ��� ���
void result(const Point* data, Point* original, const int data_arr_max, const Centroid* centroid, const int n_clusters, const int max_index);

#endif // !__KLUSTERING_H__
