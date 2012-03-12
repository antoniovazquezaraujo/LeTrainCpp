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
	void toggleLink();
	virtual void addToTrain(int, Train *)=0;

	void reverseImpulse();
	bool gotoRail(Rail * r);
	bool goBackToRail(Rail * r);
	void forward();
	void backward();

	void setMoved(bool moved);
	bool isSelected();
	void setSelected(bool selected);
	bool isMoved();
	float getImpulse();
	float receiveImpulse(float imp=0.1, Dir d=Dir::NO_DIR);
	void consumeImpulse();
	void generateImpulse();
	void incImpulseGenerated(float n=0.1);
	void decImpulseGenerated(float n=0.1);
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

private:
	float impulse;
	float impulseGenerated;
	int brakes;
	int mass;
	float speed;
	float friction;
	Rail * rail;
	static Logger log;
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
