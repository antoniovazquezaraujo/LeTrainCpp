#ifndef RAIL_H
#define RAIL_H
#include "vehicle.h"
class Vehicle;
class Finder;
class Point;
class Commander;
class Rail;
class Cross;
class Charmap;
class Railmap;
class Sim;
class Node;
class List;
class Iterator;
enum{
	STOP_RAIL, 
	VERTICAL_RAIL,
	HORIZONTAL_RAIL,
	DOWN_DIAGONAL_RAIL,
	UP_DIAGONAL_RAIL,
	CURVE_RAIL,
	CROSS_RAIL
};
class Rail{
public:
	Rail (int type, int row, int col);
	~Rail();
	void setlink(int dir, Rail * link);
	Rail * link(int dir);
	void setDir(int dir, int value);
	int invertDir(int dir);
	int getDir(int dir);
	int validDir();

	int type;
	char aspect;
	Point pos; 
	int dirs[8];
	Rail * links[8];     
	Vehicle*  vehicle;
};
#endif
