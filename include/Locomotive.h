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
	void incImpulseGenerated(float n=0.1);
	void decImpulseGenerated(float n=0.1);
//	float getImpulse();
//	float receiveImpulse(float imp=0.1, Dir d=Dir::NO_DIR);
//	void generateImpulse();
//	void consumeImpulse();
private:
	int id;
	int power;
	
};
#endif
