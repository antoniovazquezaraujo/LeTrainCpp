#ifndef RAILPEN_H
#define RAILPEN_H
#include "Basic.h"
#include "RailPen.h"
#include "Vehicle.h"

class RailPen:public Vehicle{
public:
	enum RailPenMode {MOVING, PAINTING};
	RailPen(Dir dir=Dir::E);
	~RailPen();
	Rail * makeNewRail(Rail * rail);
	bool move(int distance = 1, Sim * sim =0);
	bool forward(int distance = 1);
	bool backwards(int distance = 1);
	void setMode(RailPenMode mode=MOVING);
	RailPenMode getMode();
	Dir getLastDir();
	void paint(Window * g);
	void erase(Window * g);
private:
	Dir lastDir;
	friend ostream & operator << (ostream & o, const RailPen & pen);
	RailPenMode mode;
	static Logger log;

};
#endif
