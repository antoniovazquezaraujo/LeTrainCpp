/*
  point.cpp
  A point 
  -------------------------------------
*/
#include "letrain.h"
void Point::move(int direction){
	switch(direction) {
	case N: 
		row--; 
		break;
	case NE: 
		row--; 
		col++; 
		break;
	case E: 
		col++; 
		break;
	case SE: 
		row++; 
		col++; 
		break;
	case S: 
		row++; 
		break;
	case SW: 
		row++; 
		col--; 
		break;
	case W: 
		col--; 
		break;
	case NW: 
		row--; 
		col--; 
		break;
	default:
		exit(0);
	}
}
