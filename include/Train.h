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
	Train();
	void paint(Window * g);

	//Manejo manual
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
	void setSpeed(int speed);
	int getSpeed();
	int getTotalMass();
	int move();
	void setMoved(bool moved);

	//Manejo de vehículos
	void selectNextVehicle();
	void selectPrevVehicle();
	// Estas devuelven un puntero al tren que desaparece y que debemos borrar
	// o bien al nuevo que se crea automáticamente y que debemos agregar
	//Train * linkSelectedVehicleForward();
	//Train * unlinkSelectedVehicleForward();
	//Train * linkSelectedVehicleBackward();
	//Train * unlinkSelectedVehicleBackward();

	Train * link();            // intro
	Train * unlink();          // intro
	void reverseSelector(); // barra espaciadora
	void advanceSelector(); // flecha arriba

	//Gestión de elementos
	//Si hay via libre agregamos al final del tren en la dirección en la que
	//va el selector de vehículos
	void addWagon();
	void addLocomotive();
	void addVehicle(RailVehicle* v);
	void addWagon(Wagon* w);
	void addLocomotive(Locomotive* l);

	//Borra el último vehículo en la dirección del selector de vehículos
	//Si se quiere eliminar uno del medio, dividir primero el tren en dos,
	//borrar el último y luego volver a unirlos.
	void removeLastVehicle();
	vector<RailVehicle*> & getVehicles();
	void clear();
private:
	int crash(RailVehicle * crashed, int impulse, Dir d);
	Dir getDirFromFirst();
	Dir getDirFromLast();
	void shiftForward();
	void shiftBackward();
	int sumImpulse();
	//int setupTrainDir();

//properties:
	vector <RailVehicle*> vehicles;
	Selector<vector<RailVehicle*>> vehicleSelector;
	list <Locomotive*> locomotives;
	//list<RailVehicle*>::iterator vehicleIterator;

	int totalImpulse;
	int totalMass;
	Dir trainDir;
	bool moved;
	bool reversed;
	bool selected;
	bool reversedSelector;
	static Logger log;
}; 
#endif
