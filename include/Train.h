#ifndef TRAIN_H
#define TRAIN_H
#include "Basic.h"
#include "Dir.h"
#include "Vehicle.h"
#include "Locomotive.h"
#include "Wagon.h"
#include "Rail.h"
#include "RailVehicle.h"
#include "Selector.h"
enum SelectorStatus{
	atBegin, atMiddle, atEnd
};
class Train{
public:
	typedef deque<RailVehicle *> TVehicles;
	typedef deque<Locomotive  *> TLocomotives;
	Train();
	void paint(Window * g);

	//Manejo manual
	void selectFrontLink();
	void selectBackLink();
	void link();
	Train * unlink();
	void setSelected(bool selected);
	bool isSelected();
	void push();
	void pull();
	void selectNextForkDir();
	void selectPrevForkDir();

	//Motores
	void incImpulseGenerated();
	void decImpulseGenerated();
	//invertimos el sentido de todo el tren
	void invert();
	void setForwardDir();
	void setBackwardDir();
	void setSpeed(float speed);
	float getSpeed();
	int getTotalMass();
	void move();
	void setMoved(bool moved);

	//Manejo de vehículos
	void selectNextVehicle();
	void selectPrevVehicle();

	void reverseSelector(); 
	void advanceSelector(); 

	TVehicles               & getVehicles();
	TLocomotives            & getLocomotives();

	Selector<TVehicles>     getVehicleSelector();
	Selector<TLocomotives>  getLocomotiveSelector();

	//Gestión de elementos
	//Si hay via libre agregamos al final del tren en la dirección en la que
	//va el selector de vehículos
	void addVehicle(int p, RailVehicle* v);
	void addWagon(int p, Wagon* w);
	void addLocomotive(int p, Locomotive* l);

	//Borra el último vehículo en la dirección del selector de vehículos
	//Si se quiere eliminar uno del medio, dividir primero el tren en dos,
	//borrar el último y luego volver a unirlos.
	void removeLastVehicle();
	void clear();
	bool isMarkedAsDeleted();
	void markAsDeleted();
private:
	int crash(RailVehicle * crashed, int impulse, Dir d);
	Dir getDirFromFirst();
	Dir getDirFromLast();
	void shiftForward();
	void shiftBackward();
	float sumImpulse();

//properties:
	TVehicles vehicles;
	Train::TLocomotives locomotives;

	Selector<TVehicles> vehicleSelector;
	Selector<TLocomotives> locomotiveSelector;

	int numStoppedTurns;
	float totalImpulse;
	float speed;
	int totalMass;
	Dir trainDir;
	bool moved;
	bool reversed;
	bool selected;
	bool reversedSelector;
	bool deleted;
	static Logger log;
	friend ostream & operator << (ostream & o, Train t);
}; 
#endif
