#ifndef VEHICLE_H
#define VEHICLE_H

class Vehicle;
#include <cstdlib> 
#include <cassert> 
#include "point.h"
#include "rail.h"
#include "persist.h"
#include "finder.h"
#include "dir.h"
#include "graphics.h"
#include "trainiterator.h"

// Vehicle types
enum{
	DEF_WAGON_BG=0,
	DEF_WAGON_FG=3,
	DEF_LOCOMOTIVE_BG=0,
	DEF_LOCOMOTIVE_FG=7,
	DEF_RAIL_BG=0,
	DEF_RAIL_FG=4,
	DEF_CROSS_FG=6,
	DEF_CROSS_BG=0,
	DEF_FINDER_FG=0,
	DEF_FINDER_BG=5
};

enum{
	LOCOMOTIVE, 
	WAGON,
	CROSS
};
class Vehicle{
public:
	Vehicle  (int type);
	~Vehicle ();
	int      newid();
	Point *  getPos();
	void     setPos(Point * pos);
	float    getImp();
	void     setImp(float imp);
	void     incImp(float imp=1);
	int      getBrakes();
	void     setBrakes(int brakes);
	int      getMass();
	void     link(Vehicle*  b);
	void     unlink(Vehicle*  b);
	int      linked(Vehicle*  b);
	int      gotoRail(Rail * r);
	int      linkedcanmove();
	int      pushedcanmove();
	int      canmove();
	int      forward();
	void     reverse();
	void     setDir(int dir);
	int      getDir();
	int      linkedMass(int dir);
	int      pushedMass();
	void     braketrain();
	float    distlinked(float imp);
	int      movelinked();
	void     orientlinked(int dir);
	float    distpushed(float imp);
	int      movepushed();

	int id;
	int type;
	int dir;
	Rail * rail;
	char aspect;
	int fg;
	int bg;
	Point pos;
	float imp;
	int power;
	int brakes;
	int mass;
	Vehicle*  links[2];
	int link0id;
	int link1id;
	int moved;
	float speed;
	int turns;
	int position;
	float friction;
	int maxTurns;
};
#endif
