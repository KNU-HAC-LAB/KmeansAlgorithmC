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
void dataToArray(const char* str_tmp, const int x, const int y, Point* data, const int index);

// Ŭ�����͸� �ڵ�
void clustering(Point* data, const int data_arr_max, const Centroid* centroid, const int n_cluster);

// ��Ŭ���� �Ÿ� ���
float euclideanDistance(Point data, Centroid centroid);

// �߽����� ��ġ ����
void toCentroidCenter(Point* data, const int data_arr_max, Centroid* centroid, const int n_cluster);

// Centroid ���� ����
Centroid* centroid_copy(Centroid* centroid, const int n_cluster);

// Centroid ���� �ݺ� �� ���� ���� ������ �� Ȯ��
int checkDoesCentroidIsSameWithBefore(Centroid* centroid, Centroid* centroid_before, const int n_cluster);

#endif // !__KLUSTERING_H__
