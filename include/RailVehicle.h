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

	bool forward(int distance = 1);
	bool backwards(int distance = 1);
	bool gotoRail(Rail * r);

	bool pushedCanMove();
	bool canMove();
	bool linkedCanMove();
	int linkedMass(Dir dir);
	int pushedMass();
	void brakeTrain();
	float distLinked(float imp);
	void setMoved(bool moved);
	bool isMoved();
	bool moveLinked();
	bool movePushed();
	void orientLinked(Dir dir);
	float distPushed(float imp);

	float getImp();
	void setImp(float imp);
	void incImp(float imp=1);
	void setSpeed(int speed);
	int getSpeed();

	int getBrakes();
	void setBrakes(int brakes);

	int getMass();

	Rail* getRail();
	void setRail(Rail * r);

	RailVehicle * getRailVehicleAt(int numLink);
	enum{MAX_TURNS=20};

private:
	Link linker;
	float imp;
	int brakes;
	int mass;
	float speed;
	float friction;
	Rail * rail;
	static Logger log;
	bool moved;

public:
	// cambiar esto!
	int turns;
	friend ostream & operator << (ostream & o, RailVehicle & v);
};
#endif
