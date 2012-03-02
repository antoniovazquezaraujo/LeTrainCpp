#ifndef SIM_H
#define SIM_H
#include "Basic.h"
#include "RailMap.h"
#include "RailPen.h"
#include "Selector.h"
class Sim{
public:
	//typedef IntMap<Locomotive   *> CLocomotives; 
	//typedef IntMap<Wagon        *> CWagons; 
	//typedef IntMap<EventProgram *> CPrograms; 
	//typedef IntMap<Sensor       *> CSensors; 
	//typedef IntMap<Semaphore    *> CSemaphores; 
	//typedef IntMap<ForkRail     *> CForks; 
	Sim();
	~Sim();

	vector<Train        * >& getTrains();
	vector<Wagon        * >& getWagons();
	vector<Locomotive   * >& getLocomotives();
	vector<Sensor       * >& getSensors();
	vector<EventProgram * >& getEventPrograms();
	vector<Semaphore    * >& getSemaphores();
	vector<ForkRail     * >& getForks();

	void addWagon         (Wagon*       );
	void addLocomotive    (Locomotive*  );
	void addTrain         (Train*       );
	void addSensor        (Sensor*      );
	void addEventProgram  (EventProgram*);
	void addFork          (ForkRail*    );
	void addSemaphore     (Semaphore*   );

	vector<Train *>::iterator getSelectedTrain();
	void selectPrevTrain();
	void selectNextTrain();
	void selectNextVehicle();
	void selectPrevVehicle();
	Train        * getTrain(        int key);
	Wagon        * getWagon(        int key);
	Locomotive   * getLocomotive(   int key);
	Sensor       * getSensor(       int key);
	EventProgram * getEventProgram( int key);
	ForkRail     * getFork(         int key);
	Semaphore    * getSemaphore(    int key);

	void removeTrain(        int key);
	void removeWagon(        int key);
	void removeLocomotive(   int key);
	void removeSensor(       int key);
	void removeEventProgram( int key);
	void removeFork(         int key);
	void removeSemaphore(    int key);

	void moveTrains();
	//void moveWagons();
	//void moveLocomotives();
	void checkSensors();

	void followVehicle(Vehicle * v);

	void acceptAndRun(Event * event, EventProgram * eventProgram);
	Finder * getFinder();
	void paint();
	void erase();
	RailMap railMap;
	RailPen railPen;
private:
	Finder  * finder;
	SimView * view;
	Selector<vector<Train*>> trainSelector;
	vector<Train        * >trains;
	vector<Wagon        * >wagons;
	vector<Locomotive   * >locomotives;
	vector<Sensor       * >sensors;
	vector<EventProgram * >eventPrograms;
	vector<Semaphore    * >semaphores;
	vector<ForkRail     * >forks;

	static Logger log;
};
#endif
