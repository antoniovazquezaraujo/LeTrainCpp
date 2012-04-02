#include "Basic.h"
#include "Vehicle.h"


Vehicle::Vehicle(Aspect * aspect)
	: aspect(aspect),pos(0,0), dir(Dir::NO_DIR){
}
Vehicle::~Vehicle(){

}
//bool Vehicle::forward(){
//	pos.move(dir);
//	return true;
//}
//bool Vehicle::backwards(){
//	pos.move(dir+MIDDLE_ANGLE);
//	return true;
//}
Point  Vehicle::getPos()const{
	return  pos;
}
void Vehicle::rotateLeft(int angle){
	dir+=angle;
}
void Vehicle::rotateRight(int angle){
	dir+=(angle);
}
void Vehicle::rotate(int angle){
	dir+=angle;
}
void Vehicle::setPos(Point pos){
	this->pos=pos;
}
void Vehicle::setRow(int row){
	this->pos.row = row;
}
void Vehicle::setCol(int col){
	this->pos.col = col;
}
void Vehicle::setDir(Dir dir){
	this->dir = dir;
}
Dir Vehicle::getDir()const{
	return dir;
}
Aspect * Vehicle::getAspect(){
	return aspect;
}
//void Vehicle::paint(Window * g){
//	g->setBg(getAspect()->getBgColor());
//	g->setFg(getAspect()->getFgColor());
//	g->putC(pos.row, pos.col, getAspect()->getAspectChar());  
//}
//void Vehicle::erase(Window * g){
//	g->setBg(getAspect()->getBgColor());
//	g->setFg(getAspect()->getFgColor());
//	g->putC(pos.row, pos.col, ' ');  
//}
ostream & operator << (ostream & o, Vehicle & v){
	o << "dir: " << v.dir.getName() << ", pos:" << v.pos.row << "," << v.pos.col;
	return o;
}
