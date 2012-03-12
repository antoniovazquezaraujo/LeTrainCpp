#ifndef FINDER_H
#define FINDER_H
#include "Basic.h"
#include "RailVehicle.h"

class Finder :public RailVehicle{
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
	void addToTrain(int, Train *);
	bool isEmpty();
private:
	Rail * rail;
	static Logger log;
};
#endif
