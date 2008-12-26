#ifndef RAIL_H
#define RAIL_H
#include "Basic.h"
#include "Point.h"
class Rail{
public:
	Rail(DirEnv * env, Aspect * aspect);
	~Rail();
	DirEnv getEnv();
	Aspect * getAspect();
	virtual Dir getPath(Dir dir);
	Dir getDir(Dir dir);
	Dir getAnyPath();
	void setRailVehicle(RailVehicle * vehicle);
	RailVehicle * getRailVehicle();
	Point getPos();
	void setPos(Point pos);
	Rail * getLinkedRailAt(Dir d);
	void linkRailAt(Dir d, Rail * rail);
	void paint(Window * g);
	void erase(Window * g);
protected:
	friend ostream & operator << (ostream & o, Rail & rail);
	Point pos;
	DirEnv * dirEnv;
	RailEnv * railEnv;
	Aspect * aspect;	
	RailVehicle * vehicle;
	static Logger log;
};
#endif
