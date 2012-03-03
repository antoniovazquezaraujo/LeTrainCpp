#ifndef RAIL_H
#define RAIL_H
#include "Basic.h"
#include "Point.h"
/*

links:
Cada puerta dice con qué rail está conectada 
[ R1][   ][   ]
[   ][ . ][ R2]
[   ][   ][   ]

dirs:
Cada puerta dice a donde va a dar.
[ E ][   ][   ]
[   ][ . ][ NW]
[   ][   ][   ]
*/
class Rail{
public:
	  		Rail();
	void 	enterVehicle  (RailVehicle* v);
	void 	exitVehicle   (RailVehicle* v);
	void 	reverseVehicle(RailVehicle* v);
protected:
	RailEnv railEnv;
	DirEnv dirEnv;
	RailVehicle * vehicle;
};
Rail::Rail(DirEnv dirEnv)
	:dirEnv(dirEnv),
	vehicle(nullptr){
}	
/*
	EnterVehicle
	Si hay un vehículo no entra y devuelve false.
	Si el rail está vacio, entra el vehículo.
	Cuando entra un vehículo, miramos su dirección, y girandola 180 grados obtenemos
	la puerta por la que está entrando (si viene en dirección sur, entra por la puerta
	norte, etc)
	Entonces, obtenemos la puerta a donde va a dar esa entrada, y le ponemos al vehículo
	esa dirección, para que cuando salga, lo haga por ahí.
*/
bool Rail::enterVehicle(RailVehicle* vehicle){
	if(this->vehicle){
		return false;
	}
	this->vehicle = vehicle;
	Dir d = -(vehicle.dir);
	Dir out = getPath(d);
	vehicle.dir = out;
	vehicle.rail = this;
	return true;
}
/*
	ExitVehicle
	Saca el vehículo hacia el rail conectado en la dirección de avance del mismo
	y lo hace entrar en el rail conectado en esa dirección 
*/
bool Rail::exitVehicle(){
	Rail * dest = getRailAt(vehicle.dir);
	if(dest->enterVehicle(this->vehicle)){
		this->vehicle = nullptr;
		return true;
	}
	return false;
}
/*
	ReverseVehicle
	Toma la dirección del vehículo, la invierte 180 grados, y ahora busca la puerta que
	está conectada desde esa dirección de entrada. Entonces le pone al vehículo esa 
	dirección.
*/
void Rail::reverseVehicle(){
	Dir d = -(vehicle.dir);
	Dir out = getPath(d);
	vehicle.dir=out;
}
Dir Rail::getPath(Dir dir){
	return dirEnv.getPath(dir);
}
Rail * Rail::getRailAt(Dir dir){
	return railEnv.getRail(dir);
	//return links[dir.value];
}
#endif
