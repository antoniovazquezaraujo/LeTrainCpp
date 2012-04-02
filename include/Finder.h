#ifndef FINDER_H
#define FINDER_H
#include "Basic.h"
#include "RailVehicle.h"
#include "Dir.h"

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
	void incImpulseGenerated(float n=0.1);
	void decImpulseGenerated(float n=0.1);
	float getImpulse();
	float receiveImpulse(float imp=0.1, Dir d=Dir::NO_DIR);
	void generateImpulse();
	void consumeImpulse();
private:
	Rail * rail;
	
};
#endif
