#include "Basic.h"
#include "Point.h"
#include "Dir.h"

void Point::move(Dir dir, int distance){
	int direction = dir.getValue();
	switch(direction) {
	case N: 
		row-=distance; 
		break;
	case NE: 
		row-=distance; 
		col+=distance; 
		break;
	case E: 
		col+=distance; 
		break;
	case SE: 
		row+=distance; 
		col+=distance; 
		break;
	case S: 
		row+=distance; 
		break;
	case SW: 
		row+=distance; 
		col-=distance; 
		break;
	case W: 
		col-=distance; 
		break;
	case NW: 
		row-=distance; 
		col-=distance; 
		break;
	default:
		assert(0);	
	}
}
