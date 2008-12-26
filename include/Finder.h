#ifndef FINDER_H
#define FINDER_H
#include "Basic.h"
#include "Vehicle.h"

class Finder :public Vehicle{
public:
	Finder (RailVehicle * r=0);
	~Finder();
	void gotoRail(Rail * r);
	bool forward(int distance=1);
	bool backwards(int distance=1);
	void reverse();
	Rail * getRail();
	Dir getDir();
	void setDir(Dir dir);
	void paint(Window * g);
	void erase(Window * g);
private:
	Rail * rail;
	static Logger log;
};
#endif
