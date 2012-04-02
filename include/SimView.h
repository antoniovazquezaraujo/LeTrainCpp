#ifndef GAMEVIEW_H
#define GAMEVIEW_H
#include "Basic.h"
#include "Point.h"

class SimView{
public:
	SimView (Sim* sim);
	~SimView();
	void paint();
	void erase();
	Point getPos();
	void followVehicle(Vehicle * v);
	void setPos(Point p);
private:
	void recalcWindowShift();
	Sim * sim;
	Point pos;
	Window * window;
	Vehicle * vehicleToFollow;
	
};
#endif
