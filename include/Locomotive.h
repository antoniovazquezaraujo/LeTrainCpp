#ifndef LOCOMOTIVE_H
#define LOCOMOTIVE_H
#include "RailVehicle.h"
class Locomotive: public RailVehicle{
public:
	Locomotive();
	~Locomotive();
	int getPower();
	void setPower(int power);
	void decPower(int dec=1);
	void incPower(int inc=1);
	void paint(Window * g);
	void erase(Window * g);
	int getId();
	void setId(int id);
	void addToTrain(Train *);
	//void move();
private:
	int id;
	int power;
	static Logger log;
};
#endif
