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
	int getImpulse();
	int receiveImpulse(int imp=1, Dir d=Dir::NO_DIR);
	void consumeImpulse();
	void generateImpulse();
	void incImpulseGenerated(int n=1);
	void decImpulseGenerated(int n=1);
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
	int impulse;
	int impulseGenerated;
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
