#include "Basic.h"
#include "Rail.h"
#include "DirEnv.h"
#include "RailVehicle.h"
#include "Window.h"
#include "Aspect.h"
MAKE_LOGGER(Rail);

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
    Dir d = -(vehicle->getDir());
    Dir out = getPath(d);
    vehicle->setDir(out);
	vehicle->setPos(pos);
    vehicle->setRail(this);
    return true;
}
/*
    ExitVehicle
    Saca el vehículo hacia el rail conectado en la dirección de avance del mismo
    y lo hace entrar en el rail conectado en esa dirección
*/
bool Rail::exitVehicle(){
    Rail * dest = getLinkedRailAt(vehicle->getDir());
    if(dest->enterVehicle(this->vehicle)){
        this->vehicle = nullptr;
        return true;
    }
    return false;
}
/*
    ReverseVehicle
    Toma la dirección del vehículo, y busca la puerta que
    está conectada desde esa dirección de entrada. Entonces le pone al vehículo esa
    dirección.
*/
void Rail::reverseVehicle(){
	vehicle->setDir(getPath(vehicle->getDir()));
}
void Rail::reverseVehicleImpulse(){
	vehicle->reverseImpulse();
}
//---------------------------------------
Rail::Rail(DirEnv * dirEnv, Aspect * aspect)
	:aspect(aspect), vehicle(0),dirEnv(*dirEnv){
}

Rail::~Rail(){
}
DirEnv Rail::getEnv(){
	return dirEnv;
}
Aspect * Rail::getAspect(){
	return aspect; 
}
void Rail::paint(Window * g){
	g->setBg(getAspect()->getBgColor());
	g->setFg(getAspect()->getFgColor());
	g->putC(pos.row, pos.col, getAspect()->getAspectChar());  
}
void Rail::erase(Window * g){
	g->setBg(getAspect()->getBgColor());
	g->setFg(getAspect()->getFgColor());
	g->putC(pos.row, pos.col, ' ');  
}
Dir Rail::getDir(Dir dir){
	return dirEnv.getDir(dir);
}
Dir Rail::getPath(Dir dir){
	return dirEnv.getPath(dir);
}
Dir Rail::getAnyPath(){
	return dirEnv.getAnyPath();
}
void Rail::setRailVehicle(RailVehicle * vehicle){
	this->vehicle = vehicle;
}
RailVehicle * Rail::getRailVehicle(){
	return vehicle;
}
Point Rail::getPos(){
	return pos;
}
void Rail::setPos(Point pos){
	this->pos = pos;
}
Rail * Rail::getLinkedRailAt(Dir dir){
	return railEnv.getRail(dir);
}
void Rail::linkRailAt(Dir dir, Rail * rail){
	railEnv.addPath(dir, rail);
	if(rail){
		if(rail->getLinkedRailAt(-dir) != this){
			rail->linkRailAt(-dir, this);
		}
	}
}
void Rail::enter(RailVehicle* v){
	setRailVehicle(v);
	Dir dir = v->getDir();
	if(dir.getValue() != Dir::NO_DIR){
		v->setDir(getPath(-dir));
	}else{
		v->setDir(getAnyPath());
	}
	v->setPos(getPos());
	v->setRail(this);
}
ostream & operator << (ostream & o, Rail & rail){
	DirEnv * env = &(rail.dirEnv);
	int n ;
	o << endl; 
	//first row
	n= Dir::NW;
	o << "[" << setw(2) << (env->getPath(n).getValue() != Dir::NO_DIR? env->getPath(n).getName():" ") << "]";
	n= Dir::N;
	o << "[" <<setw(2) <<  (env->getPath(n).getValue() != Dir::NO_DIR? env->getPath(n).getName():" ") << "]";
	n= Dir::NE;
	o << "[" << setw(2) << (env->getPath(n).getValue() != Dir::NO_DIR? env->getPath(n).getName():" ") << "]";
	o << endl; 

	// central row
	n= Dir::W;
	o << "[" << setw(2) << (env->getPath(n).getValue() != Dir::NO_DIR? env->getPath(n).getName():" ") << "]";
	// central point
	o << "( " << rail.getAspect()->getAspectChar() << ")" ; 
	n= Dir::E;
	o << "[" << setw(2) << (env->getPath(n).getValue() != Dir::NO_DIR? env->getPath(n).getName():" ") << "]";
	o << endl; 

	// third row
	n= Dir::SW;
	o << "[" << setw(2) << (env->getPath(n).getValue() != Dir::NO_DIR? env->getPath(n).getName():" ") << "]";
	n= Dir::S;
	o << "[" << setw(2) << (env->getPath(n).getValue() != Dir::NO_DIR? env->getPath(n).getName():" ") << "]";
	n= Dir::SE;
	o << "[" << setw(2) << (env->getPath(n).getValue() != Dir::NO_DIR? env->getPath(n).getName():" ") << "]";
	o << endl; 
	return o;
}
