#ifndef RAILVEHICLE_H
#define RAILVEHICLE_H
#include "Basic.h"
#include "RailVehicle.h"
#include "Vehicle.h"
#include "Link.h"
class RailVehicle :public Vehicle{
public:
	RailVehicle (Aspect * aspect);
	~RailVehicle ();
	RailVehicle * getVehicleLinkedWith(RailVehicle * r);
	RailVehicle * getVehicleLinkedAt(int numLink);
	bool interLink(RailVehicle * b);
	bool link(RailVehicle* b);
	void unlink(RailVehicle* b);
	void setLink(Dir d);
	void setUnlink(Dir d);
	int isLinked(RailVehicle* b);
	virtual void addToTrain(Train *)=0;

	bool gotoRail(Rail * r);
	bool goBackToRail(Rail * r);
	void forward();
	void backward();

	void setMoved(bool moved);
	bool isSelected();
	void setSelected(bool selected);
	bool isMoved();
	int getImpulse();
	void receiveImpulse(int imp=1, Dir d=Dir::NO_DIR);
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
	Link linker;
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

public:
	// cambiar esto!
	int turns;
	friend ostream & operator << (ostream & o, RailVehicle & v);
};
#endif
