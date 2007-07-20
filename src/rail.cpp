/*
  rail.cpp
  The railroad is a graph of rails 
  -------------------------------------
*/
#include "letrain.h"

Rail::Rail(int type, int row, int col){
	this->type = type;
	pos.row = row;
	pos.col = col;
	vehicle = NULL;
	int cont = 0;
	for(cont = 0; cont < 8; cont++){
		dirs[cont] =NO_DIR; 
		links[cont] = NULL; 
	}
	switch (type){
	case STOP_RAIL:	
		aspect = ':';
		break;
	case CURVE_RAIL:
		aspect = '.';
		break;
	case CROSS_RAIL:
		aspect = '+';
		break;                           
	case VERTICAL_RAIL:	                         
		setDir(N, S);
		setDir(S, N);
		aspect = '|';
		break;                           
	case HORIZONTAL_RAIL:	                         
		setDir(W, E);
		setDir(E, W);
		aspect = '-';
		break;                           
	case DOWN_DIAGONAL_RAIL:	                         
		setDir(NW, SE);
		setDir(SE, NW);
		aspect = '\\';
		break;                           
	case UP_DIAGONAL_RAIL:	                         
		setDir(SW, NE);
		setDir(NE, SW);
		aspect = '/';
		break;
	}
}

Rail::~Rail(){

}
void Rail::setlink(int dir, Rail * linked){
	assert(dir >=0);
	assert(dir <8);
	links[dir] = linked;
}
Rail *  Rail::link(int dir){
	if(dir < 0) {
		return NULL;
	}
	if(dir >=8 ) {
		return NULL;
	}
	assert(dir >=0);
	assert(dir <8);
	return links[dir];
}
void Rail::setDir(int dir, int value){
	assert(dir >=0);
	assert(dir <8);
	dirs[dir] = value;
}
int Rail::getDir(int dir){
	if(dir < 0) {
		return NO_DIR;
	}
	if(dir >=8 ){
		return NO_DIR;
	}
	return dirs[dir];
}
int Rail::invertDir(int dir){
	if(dir != NO_DIR){
		int invdir = getDir(dir);
		return invdir;
	}else{
		return NO_DIR;
	}
}
int Rail::validDir(){
	int cont = 0;
	for(cont = 0; cont < 8; cont++){
		if((dirs[cont]) != NO_DIR){ 
			return cont;
		}
	}
	return NO_DIR;
}
