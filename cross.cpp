/*
  cross.cpp
  A cross point 
  -------------------------------------
*/
#include "letrain.h"
Cross::Cross (Rail * rail, int in, int outs[]){
	assert(rail->type == CROSS_RAIL);
	this->rail = rail;
	this->in= in;
	this->out= 0;
	this->outs[0] = outs[0];
	this->outs[1] = outs[1];
	this->outs[2] = outs[2];
}
Cross::~Cross(){

}
void Cross::setDir(int index){
	out= index;
	rail->dirs[in] = outs[out];
	updateRail();
}
void Cross::nextDir(){
	out++;
	out %= 3;
	while(outs[out] == NO_DIR){ 
		out++;
		out %= 3;
	}
	setDir(out);
}
void Cross::updateRail(){
	char rc [] ={
		':',
		'-',
		'/', 
		'|',
		'\\' ,
		'-', 
		'/',
		'|',
		'\\',
	};
	rail->aspect= rc[rail->dirs[in]+1]; // +1 because NO_DIR is -1
}
int Cross::getDir(){
	return out;
}
int Cross::keyDir(){
	return in;
}
Rail * Cross::getRail(){
	return rail;
}
