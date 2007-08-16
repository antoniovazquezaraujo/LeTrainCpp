#ifndef FINDER_H
#define FINDER_H

class Finder;
#include "vehicle.h"
#include "rail.h"
#include "point.h"
#include "dir.h"
class Vehicle;
class Rail;
class Finder{
public:
	Finder (Vehicle*  v);
	~Finder();
	void gotoRail(Rail * r);
	int forward();
	void reverse();

	int dir;
	Rail * rail;
	char aspect;
	int fg;
	int bg;
	Point pos;
};
#endif
