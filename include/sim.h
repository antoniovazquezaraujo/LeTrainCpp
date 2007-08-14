#ifndef SIM_H
#define SIM_H
#include "vehicle.h"
#include "vehicle.h"
#include "rail.h"
#include "cross.h"
#include "list.h"
#include "iterator.h"
#include "commander.h"
#include "railmap.h"
class Sim{
public:
	Sim        ();
	~Sim       ();
	void       setup();
	void       free();
	int        loadmap(char * filaname);
	int        loadgame(char * filaname);
	int        save();
	void       start();
	void       stop();
	Vehicle*   vehiclebyid(int id);
	void       linkvehicles();
	Vehicle*   selVehicle(int vehicleindex);
	Vehicle*   getVehicle(); //selected vehicle

	// operations with the selected vehicle
	void       incpower();
	void       decpower();
	int        incBrakes();
	int        decBrakes();
	int        reverse();
	int        link();
	int        unlink();
	void       del(); 

	// adding vehicles
	void       addVehicle(Vehicle*  v);
	Vehicle*   addloco();
	Vehicle*   addwagon();

	//selecting vehicles
	Vehicle*   nextloco();
	Vehicle*   prevloco();

	//selecting rails
	Rail *     getRail();

	//operations with the map
	void       selectMapRow(int row);
	int        getMapRow();
	void       selectMapCol(int col);
	int        getMapCol();

	//operations with crosses
	Cross *    nextCross();
	Cross *    prevCross();
	Cross *    getCross();
	void       changeSelectedCross(); 

	//
	void       movetrains();
	void       cycle();
	void       doactions();
	void       draw();
	void       drawhelp();
	void       updatevisor();
	void       drawstatus();
	void       drawfinder();
	void       drawVehicles();
	void       drawrails();
	void       drawcrosses();
	void       clearrails();

	Railmap * railmap;
	Commander *  commander;
	Vehicle*  vehicle;
	Cross * cross;
	int maprow;
	int mapcol;
	int maprows;
	int mapcols;
	int statusrow;
	int statuscol;
	int statusrows;
	int statuscols;
	Rail * rail;
	List * locomotives;
	Iterator * i_locomotives;
	Iterator * i_crosses;
	List * vehicles;
};
#endif
