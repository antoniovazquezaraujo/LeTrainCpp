#ifndef LINK_H
#define LINK_H
#include "Basic.h"
class Link{
public:
	Link();
	bool link(RailVehicle * r);
	RailVehicle * unlink(RailVehicle * r=0);
	bool isLinked(RailVehicle * r);
	bool isEmpty();
	bool isFull();
	RailVehicle * getVehicleLinkedWith(RailVehicle * one, RailVehicle * other);
	RailVehicle * getVehicleLinkedAt(int numLink);
private:
	array<RailVehicle *, 2>links;
	static Logger log;
};
#endif
