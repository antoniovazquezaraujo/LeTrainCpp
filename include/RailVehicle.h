#ifndef RAILVEHICLE_H
#define RAILVEHICLE_H
#include "Basic.h"
#include "Vehicle.h"
class RailVehicle :public Vehicle{
public:
	RailVehicle (Aspect * aspect);
	~RailVehicle ();
	void selectFrontLink();
	void selectBackLink();
	void link();

	void reverseImpulse();
	bool gotoRail(Rail * r);
	bool goBackToRail(Rail * r);
	void forward();
	void backward();

	void setMoved(bool moved);
	bool isSelected();
	void setSelected(bool selected);
	bool isMoved();

	virtual void generateImpulse();
	virtual float getImpulse();
	virtual void consumeImpulse();
	virtual float receiveImpulse(float imp=0.1, Dir d=Dir::NO_DIR);

	virtual void incImpulseGenerated(float n=0.1)=0;
	virtual void decImpulseGenerated(float n=0.1)=0;
	void setSpeed(int speed);
	int getSpeed();

	int getBrakes();
	void setBrakes(int brakes);

	Train * getTrain();
	void setTrain(Train *);

	int getMass();

	Rail* getRail();
	void setRail(Rail * r);

	RailVehicle * getRailVehicleAt(int numLink);
	enum{MAX_TURNS=20};

protected:
	float impulseGenerated;
	float impulse;
	int mass;
	float kinetic;
private:
	int brakes;
	float speed;
	float friction;
	Rail * rail;
	
	bool moved;
	bool selected;
	Train * train;
	int selectedLink;

public:
	// cambiar esto!
	int turns;
	friend ostream & operator << (ostream & o, RailVehicle & v);
};
#endif
