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

void dataToArray(const char* str_tmp, const int x, const int y, const int index);

#endif // !__KLUSTERING_H__
