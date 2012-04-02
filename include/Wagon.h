#ifndef WAGON_H
#define WAGON_H
#include "RailVehicle.h"
#include "Window.h"
class Wagon: public RailVehicle{
public:
	Wagon();
	~Wagon();
	void incImpulseGenerated(float n=0.1);
	void decImpulseGenerated(float n=0.1);

//	float receiveImpulse(float imp=0.1, Dir d=Dir::NO_DIR);
//	void generateImpulse();
//	void consumeImpulse();
//	float getImpulse();
	void paint(Window * g);
	void erase(Window * g);
private:
	
};
#endif
