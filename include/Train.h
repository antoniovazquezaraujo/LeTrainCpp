#ifndef TRAIN_H
#define TRAIN_H
#include "Basic.h"
class Train{
public:
	Train(Locomotive * locomotive);
	void moveTrain();
	void addWagon(RailVehicle * wagon);
	void addLocomotive(Locomotive * locomotive);
	void paint(Window * g);
	void setSpeed(int speed);
private:
	vector<RailVehicle *> wagons;
	vector<Locomotive *> locomotives;
	vector<RailVehicle*> allVehicles;
	static Logger log;
}; 
#endif

