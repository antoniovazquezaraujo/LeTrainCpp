#ifndef VEHICLE_H
#define VEHICLE_H
#include "point.h"
#include "rail.h"
// Vehicle types
#define DEF_WAGON_BG 0 
#define DEF_WAGON_FG 3 
#define DEF_LOCOMOTIVE_BG 0 
#define DEF_LOCOMOTIVE_FG 7 
#define DEF_RAIL_BG 0 
#define DEF_RAIL_FG 4  
#define DEF_CROSS_FG 6 
#define DEF_CROSS_BG 0 
#define DEF_FINDER_FG 0 
#define DEF_FINDER_BG 5 
enum{
	LOCOMOTIVE, 
	WAGON,
	CROSS
};
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
class Vehicle{
public:
	Vehicle    (int type);
	~Vehicle   ();
	int        newid();
	Point *    getPos();
	void       setPos(Point * pos);
	float      getImp();
	void       setImp(float imp);
	void       incImp(float imp=1);
	int        getBrakes();
	void       setBrakes(int brakes);
	int        getMass();
	void       link(Vehicle*  b);
	void       unlink(Vehicle*  b);
	int        linked(Vehicle*  b);
	int        gotoRail(Rail * r);
	int        linkedcanmove();
	int        pushedcanmove();
	int        canmove();
	int        forward();
	void       reverse();
	void       setDir(int dir);
	int        getDir();
	int        linkedMass(int dir);
	int        pushedMass();
	void       braketrain();
	float      distlinked(float imp);
	int        movelinked();
	void       orientlinked(int dir);
	float      distpushed(float imp);
	int        movepushed();

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
