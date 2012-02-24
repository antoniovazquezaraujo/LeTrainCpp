#include "Basic.h"
#include "Point.h"
#include "Dir.h"
Dir Point::locate (Point p){
	if(row > p.row){
		if(col > p.col){
			return Dir::NW;
		}else if(col < p.col){
			return Dir::NE;
		}else{
			return Dir::N;
		}
	}else if(row < p.row){
		if(col > p.col){
			return Dir::SW;
		}else if(col < p.col){
			return Dir::SE;
		}else{
			return Dir::S;
		}
	}else{//row == p.row
		if(col > p.col){
			return Dir::W;
		}else if(col < p.col){
			return Dir::E;
		}else{
			return Dir::NO_DIR;
		}
	}
	return Dir::NO_DIR;
}
void Point::move(Dir dir, int distance){
	int direction = dir.getValue();
	switch(direction) {
	case Dir::N: 
		row-=distance; 
		break;
	case Dir::NE: 
		row-=distance; 
		col+=distance; 
		break;
	case Dir::E: 
		col+=distance; 
		break;
	case Dir::SE: 
		row+=distance; 
		col+=distance; 
		break;
	case Dir::S: 
		row+=distance; 
		break;
	case Dir::SW: 
		row+=distance; 
		col-=distance; 
		break;
	case Dir::W: 
		col-=distance; 
		break;
	case Dir::NW: 
		row-=distance; 
		col-=distance; 
		break;
	default:
		assert(0);	
	}
}
