#ifndef POINT_H
#define POINT_H
class Point;
#include "dir.h"
#include <cassert> 

class Point{
public:
	Point(int row=0, int col=0)
		: row(row), col(col){
	}
	int row;
	int col;
	void  move(int dir);
};
#endif
