#ifndef SIM_H
#define SIM_H
#include "Basic.h"
#include "RailMap.h"
#include "RailPen.h"
#include "Selector.h"
class Sim{
public:
	typedef deque<Train        *> TTrains; 
	typedef deque<Locomotive   *> TLocomotives; 
	typedef deque<Wagon        *> TWagons; 
	typedef deque<EventProgram *> TPrograms; 
	typedef deque<Sensor       *> TSensors; 
	typedef deque<Semaphore    *> TSemaphores; 
	typedef deque<ForkRail     *> TForks; 
	Sim();
	~Sim();

	TTrains                & getTrains();
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

	void removeVehicle (RailVehicle *  );

	Sim::TTrains::iterator getSelectedTrain();
	void selectPrevTrain();
	void selectNextTrain();
	void selectNextVehicle();
	void selectPrevVehicle();
	void selectFrontLink();
	void selectBackLink();
	void link();
	Train * unlink();
	void linkTrain();
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

	Selector<TTrains> * getTrainSelector();
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
	Selector<TTrains>      trainSelector;
	TTrains                trains;
	vector<Wagon        * >wagons;
	vector<Locomotive   * >locomotives;
	vector<Sensor       * >sensors;
	vector<EventProgram * >eventPrograms;
	vector<Semaphore    * >semaphores;
	vector<ForkRail     * >forks;

	static Logger log;
};
#endif
