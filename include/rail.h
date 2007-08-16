#ifndef RAIL_H
#define RAIL_H

class Rail;
#include <cassert>
#include "vehicle.h"
#include "dir.h"

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
	Rail(int type=STOP_RAIL, int row=0, int col=0);
	~Rail();
	void setlink(int dir, Rail * link);
	Rail * link(int dir);
	void setDir(int dir, int value);
	int invertDir(int dir);
	int getDir(int dir);
	int validDir();

	Vehicle *  vehicle;
	int type;
	char aspect;
	Point pos; 
	int dirs[8];
	Rail * links[8];
	friend bool operator==(Rail &a, Rail &b);
};
#endif
