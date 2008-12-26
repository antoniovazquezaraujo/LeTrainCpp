#include "Basic.h"
#include "Locomotive.h"
#include "LocomotiveAspect.h"
#include "Window.h"

MAKE_LOGGER(Locomotive);

Locomotive::Locomotive()
	:RailVehicle(new LocomotiveAspect()){

}
Locomotive::~Locomotive(){

}
int Locomotive::getId(){
	return id;
}
void Locomotive::decPower(int dec){
	power-=dec;
}
void Locomotive::incPower(int inc){
	power+=inc;
}
void Locomotive::setId(int id){
	this->id = id;
}
int Locomotive::getPower(){
	return power;
}
void Locomotive::setPower(int power){
	this->power = power;
} 
void Locomotive::paint(Window * g){
	g->setBg(getAspect()->getBgColor());
	g->setFg(getAspect()->getFgColor());
	g->putC(pos.row, pos.col, getAspect()->getAspectChar());  
}
void Locomotive::erase(Window * g){
	g->setBg(getAspect()->getBgColor());
	g->setFg(getAspect()->getFgColor());
	g->putC(pos.row, pos.col, ' ');  
}

void Locomotive::move(){
	if(!getRail()) return;
	incImp(getPower());
	LOG_DEBUG(log," incremento impulso a " << getImp());

	setMoved(false);

	if(getImp() > 0){
		LOG_DEBUG(log," El tren tiene impulso");
		//Esto no funciona de momento
		// Eliminar setSpeed y usar setPower y brakes!!!!!!!
		//setSpeed(getImp() / getMass());
		LOG_DEBUG(log," Ponemos velocidad a " << getSpeed());
		if(turns > 0 ){
			turns--;
			LOG_DEBUG(log," Decremento turnos del tren a " << turns);
		}else{
			LOG_DEBUG(log," Ya no le quedan turnos");
			float remainingImp = getImp();
			if(remainingImp > 0){
				LOG_DEBUG(log," queda energia para linkeds");
				remainingImp = distLinked(remainingImp);
			}
			if(remainingImp > 0){
				LOG_DEBUG(log," queda para pusheds");
				remainingImp = distPushed(remainingImp);
			}
			if(remainingImp > 0){
				LOG_DEBUG(log," queda para mi");
				setImp(remainingImp);
			}
			if(getImp() > 0){
				LOG_DEBUG(log," si aun queda algo");
				if(pushedCanMove() && linkedCanMove()){
					LOG_DEBUG(log," si se pueden mover todos los muevo");
					movePushed();
					moveLinked();
				}
			}
			if(getSpeed() > 0){
				turns = RailVehicle::MAX_TURNS* 1/getSpeed();
				LOG_DEBUG(log," como velocidad es "  << getSpeed() << " ponemos turnos a " << turns);
			}else{
				LOG_DEBUG(log," no le queda velocidad, ponemos turnos a " << turns);
				turns = RailVehicle::MAX_TURNS;
			}
		}
	}
}
