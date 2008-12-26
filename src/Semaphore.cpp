#include "Semaphore.h"
#include "Window.h"
#include "Rail.h"
Semaphore::Semaphore(int id, bool open)
	:id(id), openValue(open){
	}
int Semaphore::getId(){
	return id;
}
void Semaphore::setId(int id){
	this->id = id;
}
bool Semaphore::isOpen(){
	return openValue;
}
void Semaphore::close(){
	openValue = false;
}
Point Semaphore::getPos(){
	return pos;
}
void Semaphore::open(){
	openValue = true;
}	
void Semaphore::setRow(int row){
	pos.row = row;
}
void Semaphore::setCol(int col){
	pos.col= col;
}
void Semaphore::paint(Window * g){
	g->setBg(0);
	g->setFg(openValue?1:10);
	g->putC(getPos().row, getPos().col, ':');  
}
void Semaphore::erase(Window * g){
	g->setBg(0);
	g->setFg(openValue?1:10);
	g->putC(getPos().row, getPos().col, ' ');  
}

