#ifndef SIM_H
#define SIM_H

#include "Basic.h"
class Sim{
public:
	typedef map<int, Locomotive*> CLocomotives; 
	typedef map<int, Wagon*> CWagons; 
	typedef map<int, EventProgram*> CPrograms; 
	typedef map<int, Sensor*> CSensors; 
	typedef map<int, Semaphore*> CSemaphores; 
	typedef map<int, ForkRail*> CForks; 
	Sim(Game * game);
	~Sim();
	RailPen * getRailPen();
	RailMap * getRailMap();

	CWagons *  getWagons();
	CLocomotives * getLocomotives();
	CSensors * getSensors();
	CPrograms * getEventPrograms();
	CSemaphores * getSemaphores();
	CForks * getForks();

	void addWagon         (Wagon*,        int key=-1);
	void addLocomotive    (Locomotive*,   int key=-1);
	void addSensor        (Sensor*,       int key=-1);
	void addEventProgram  (EventProgram*, int key=-1);
	void addFork          (ForkRail*,     int key=-1);
	void addSemaphore     (Semaphore*,    int key=-1);

	Wagon*         getWagon(        int key);
	Locomotive*    getLocomotive(   int key);
	Sensor *       getSensor(       int key);
	EventProgram * getEventProgram( int key);
	ForkRail *     getFork(         int key);
	Semaphore *    getSemaphore(    int key);

	void removeWagon(        int key);
	void removeLocomotive(   int key);
	void removeSensor(       int key);
	void removeEventProgram( int key);
	void removeFork(         int key);
	void removeSemaphore(    int key);

	void moveTrains();
	void moveWagons();
	void moveLocomotives();
	void checkSensors();

	void followVehicle(Vehicle * v);

	void acceptAndRun(Event * event, EventProgram * eventProgram);
	Finder * getFinder();
	void paint();
	void erase();
private:
	Game * game;
	RailMap * railMap;
	RailPen * railPen;
	Finder * finder;
	SimView * view;

	CWagons wagons;
	CLocomotives locomotives;
	CSensors sensors;
	CPrograms eventPrograms;
	CForks forks;
	CSemaphores semaphores;

	static Logger log;
};
#endif
