#ifndef POINT_H
#define POINT_H
#include "Basic.h"
class Point{
public:
	Point(int row=0, int col=0)
		: row(row), col(col){
	}
	int row;
	int col;
	void  move(Dir dir, int distance = 1);
	Dir locate (Point p);
};
#endif
