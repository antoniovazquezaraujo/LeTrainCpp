#ifndef SIM_H
#define SIM_H
#include "Basic.h"
#include "RailMap.h"
#include "RailPen.h"
#include "Selector.h"
class Sim{
public:
	typedef deque<Train        *> TTrains; 
	typedef deque<EventProgram *> TPrograms; 
	typedef deque<Sensor       *> TSensors; 
	typedef deque<Semaphore    *> TSemaphores; 
	typedef deque<ForkRail     *> TForks; 
	Sim();
	~Sim();

	TTrains                & getTrains();
	vector<Sensor       * >& getSensors();
	vector<EventProgram * >& getEventPrograms();
	vector<Semaphore    * >& getSemaphores();
	vector<ForkRail     * >& getForks();

	void addTrain         (Train*       );
	void addSensor        (Sensor*      );
	void addEventProgram  (EventProgram*);
	void addFork          (ForkRail*    );
	void addSemaphore     (Semaphore*   );

	Sim::TTrains::iterator getSelectedTrain();
	void selectPrevTrain();
	void selectNextTrain();
	void selectFrontLink();
	void selectBackLink();
	void link();
	Train * unlink();
	void linkTrain();
	Train        * getTrain(        int key);
	Sensor       * getSensor(       int key);
	EventProgram * getEventProgram( int key);
	ForkRail     * getFork(         int key);
	Semaphore    * getSemaphore(    int key);

	void removeTrain(        int key);
	void removeSensor(       int key);
	void removeEventProgram( int key);
	void removeFork(         int key);
	void removeSemaphore(    int key);

	Selector<TTrains> * getTrainSelector();
	void moveTrains();
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
	vector<Sensor       * >sensors;
	vector<EventProgram * >eventPrograms;
	vector<Semaphore    * >semaphores;
	vector<ForkRail     * >forks;

	
};
#endif
