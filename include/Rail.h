#ifndef RAIL_H
#define RAIL_H
#include "Basic.h"
#include "Point.h"
#include "DirEnv.h"
#include "RailEnv.h"
class Rail{
public:
	Rail(DirEnv * env, Aspect * aspect);
	~Rail();

	DirEnv        getEnv();
	Aspect *      getAspect();
	virtual Dir   getPath(Dir dir);
	Dir           getDir(Dir dir);
	Dir           getAnyPath();
	RailVehicle * getRailVehicle();
	Point         getPos();
	Rail *        getLinkedRailAt(Dir d);

	void setRailVehicle(RailVehicle * vehicle);
	void setPos(Point pos);
	void linkRailAt(Dir d, Rail * rail);
	void paint(Window * g);
	void erase(Window * g);
	void operator <(RailVehicle* v);
protected:
	Point         pos;
	DirEnv        dirEnv;
	RailEnv       railEnv;
	Aspect *      aspect;	
	RailVehicle * vehicle;

	friend ostream & operator << (ostream & o, Rail & rail);
	static Logger log;
};
#endif
