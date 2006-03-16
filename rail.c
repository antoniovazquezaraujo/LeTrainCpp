/*
  rail.c
  The railroad is a graph of rails 
  -------------------------------------
*/
#include "letrain.h"

Rail r_new(int type, int row, int col){
	Rail rail = malloc(sizeof(struct rail));
	rail->type = type;
	rail->pos.row = row;
	rail->pos.col = col;
	rail->vehicle = NULL;
	int cont = 0;
	for(cont = 0; cont < 8; cont++){
		rail->dirs[cont] =NO_DIR; 
		rail->links[cont] = NULL; 
	}
	switch (rail->type){
	case STOP_RAIL:	
		rail->aspect = ':';
		break;
	case CURVE_RAIL:
		rail->aspect = '.';
		break;
	case CROSS_RAIL:
		rail->aspect = '+';
		break;                           
	case VERTICAL_RAIL:	                         
		r_setdir(rail, N, S);
		r_setdir(rail, S, N);
		rail->aspect = '|';
		break;                           
	case HORIZONTAL_RAIL:	                         
		r_setdir(rail, W, E);
		r_setdir(rail, E, W);
		rail->aspect = '-';
		break;                           
	case DOWN_DIAGONAL_RAIL:	                         
		r_setdir(rail, NW, SE);
		r_setdir(rail, SE, NW);
		rail->aspect = '\\';
		break;                           
	case UP_DIAGONAL_RAIL:	                         
		r_setdir(rail, SW, NE);
		r_setdir(rail, NE, SW);
		rail->aspect = '/';
		break;
	}
	return rail;
}

void r_free(Rail  rail){
	free(rail);
}
void r_setlink(Rail rail, int dir, Rail linked){
	assert(dir >=0);
	assert(dir <8);
	rail->links[dir] = linked;
}
Rail r_link(Rail rail, int dir){
	if(dir < 0) {
		return NULL;
	}
	if(dir >=8 ) {
		return NULL;
	}
	assert(dir >=0);
	assert(dir <8);
	return rail->links[dir];
}
void r_setdir(Rail rail, int dir, int value){
	assert(dir >=0);
	assert(dir <8);
	rail->dirs[dir] = value;
}
int r_dir(Rail rail, int dir){
	//assert(dir >=0);
	//assert(dir <8);
	if(dir < 0) {
		lg("r_dir",1,"dir < 0");	
		return NO_DIR;
	}
	if(dir >=8 ){
		lg("r_dir",1,"dir > 8");	
		return NO_DIR;
	}
	return rail->dirs[dir];
}
int r_invertdir(Rail rail, int dir){
	if(dir != NO_DIR){
		int invdir = r_dir(rail, dir);
		return invdir;
	}else{
		return NO_DIR;
	}
}
int r_validdir(Rail rail){
	int cont = 0;
	for(cont = 0; cont < 8; cont++){
		if((rail->dirs[cont]) != NO_DIR){ 
			return cont;
		}
	}
	return NO_DIR;
}
Cross x_new(Rail rail, int in, int outs[]){
	assert(rail->type == CROSS_RAIL);
	Cross ret = malloc(sizeof(struct cross));
	ret->rail = rail;
	ret->in= in;
	ret->out= 0;
	ret->outs[0] = outs[0];
	ret->outs[1] = outs[1];
	ret->outs[2] = outs[2];
	return ret;
}
void x_free(Cross cross){
	free(cross);
}
void x_setdir(Cross cross, int index){
	cross->out= index;
	cross->rail->dirs[cross->in] = cross->outs[cross->out];
	x_updaterail(cross);
}
void x_nextdir(Cross cross){
	cross->out++;
	cross->out %= 3;
	/*
	while(cross->outs[cross->out] == NO_DIR){ 
		cross->out++;
		cross->out %= 3;
	}
	*/
	x_setdir(cross, cross->out);
}
void x_updaterail(Cross cross){
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
	cross->rail->aspect= rc[cross->rail->dirs[cross->in]+1]; // +1 because NO_DIR is -1
}
int x_dir(Cross cross){
	return cross->out;
}
int x_keydir(Cross cross){
	return cross->in;
}
Rail x_rail(Cross cross){
	return cross->rail;
}
void p_move(Point  point, int direction){
	switch(direction) {
	case N: 
		point->row--; 
		break;
	case NE: 
		point->row--; 
		point->col++; 
		break;
	case E: 
		point->col++; 
		break;
	case SE: 
		point->row++; 
		point->col++; 
		break;
	case S: 
		point->row++; 
		break;
	case SW: 
		point->row++; 
		point->col--; 
		break;
	case W: 
		point->col--; 
		break;
	case NW: 
		point->row--; 
		point->col--; 
		break;
	default:
		exit(0);
	}
}
int d_invert(int direction){
	int ret = NO_DIR; 
	if(direction != NO_DIR){
		ret = (direction +4);
		if (ret > 7){
			ret -= 8;
		}
	}
	return ret;
}
