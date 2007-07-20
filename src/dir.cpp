/*
  dir.cpp
  A direction 
  -------------------------------------
*/
#include "letrain.h"
int invert(int direction){
	int ret = NO_DIR; 
	if(direction != NO_DIR){
		ret = (direction +4);
		if (ret > 7){
			ret -= 8;
		}
	}
	return ret;
}
