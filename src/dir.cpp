/*
  dir.cpp
  A direction 
  -------------------------------------
*/
#include "dir.h"
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

void dirtostr(int dir, char* dest){
	const char *dirnames[] = {"E","NE","N","NW","W","SW","S","SE","NO_DIR"};
	if(dir != NO_DIR){
		strcpy(dest, dirnames[dir]);
	}else{
		strcpy(dest, dirnames[8]);
	}
}
 int strToDir(char *c){
	int ret = 0;
	if(     !strcmp(c,"E" )) ret = E;
	else if(!strcmp(c,"NE")) ret = NE;
	else if(!strcmp(c,"N" )) ret = N;
	else if(!strcmp(c,"NW")) ret = NW;
	else if(!strcmp(c,"W" )) ret = W;
	else if(!strcmp(c,"SW")) ret = SW;
	else if(!strcmp(c,"S" )) ret = S;
	else if(!strcmp(c,"SE")) ret = SE;
	else ret = NO_DIR;
	return ret;
}


