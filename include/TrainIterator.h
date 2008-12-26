#ifndef TRAINITERATOR_H
#define TRAINITERATOR_H 
#include "Basic.h"
#include "Dir.h"
#include "Finder.h"

class TrainIterator{
public:
	TrainIterator(RailVehicle*  v, Dir dir);
	~TrainIterator();
	void reset();
	int hasNext();
	RailVehicle*  next();
private:
	RailVehicle*  start;
	Finder* finder;
	RailVehicle*  actual;
	RailVehicle*  nextVehicle;
	RailVehicle*  prev;
	Dir dir;
	static Logger log;
};
#endif
