#ifndef COMMANDER_H
#define COMMANDER_H
#include "sim.h"
#include "finder.h"
// Commander modes
enum{
	DRIVERS_MODE, 
	POINTS_MODE,
	TRAINS_MODE
};
class Sim;
class Commander{
public:
	Commander  ();
	~Commander ();
	void c_setsim(Sim * s);
	void c_key(char key);

	Sim * sim;
	Finder* finder;
	int mode;
};
#endif
