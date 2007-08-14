#ifndef TRAINITERATOR_H
#define TRAINITERATOR_H 

#include "vehicle.h"

class TrainIterator{
public:
	TrainIterator(Vehicle*  v, int dir);
	~TrainIterator();
	void reset();
	int hasNext();
	Vehicle*  next();
private:
	Vehicle*  start;
	Finder*   f;
	Vehicle*  actual;
	Vehicle*  nextVehicle;
	Vehicle*  prev;
	int dir;
};
#endif
