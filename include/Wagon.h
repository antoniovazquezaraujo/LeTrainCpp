#ifndef WAGON_H
#define WAGON_H
#include "RailVehicle.h"
#include "Window.h"
class Wagon: public RailVehicle{
public:
	Wagon();
	~Wagon();
	void paint(Window * g);
	void erase(Window * g);
private:
	static Logger log;
};
#endif
