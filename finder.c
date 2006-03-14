/*
  finder.c
  The rail pointer 
  -------------------------------------
*/
#include "letrain.h"

Finder f_new(Vehicle v){
	Finder f = malloc(sizeof(struct finder));
	f->pos.row=0;
	f->pos.col=0;
	f->fg = 4;
	f->bg = 0;
	if(v != NULL){
		f->rail = v->rail;
		f->dir = v->dir;
	}else{
		f->rail = NULL;
		f->dir = NO_DIR;
	}
	return f;
}
void f_free(Finder f){
	free(f);
}
void f_gotorail(Finder f, Rail r){
	int inputDir = NO_DIR;
	if(f->dir != NO_DIR){
		inputDir = d_invert(f->dir);
	}else{
		inputDir = r_validdir(r);
	}
	f->dir = r_dir(r, inputDir);
	f->pos.row = r->pos.row;
	f->pos.col = r->pos.col;
	f->rail = r;
}
int f_forward(Finder f){
	int ret = 0;
	Rail next = r_link(f->rail, f->dir);
	if( next != NULL){
		f_gotorail(f, next);
		ret = 1;
	}
	return ret;
}
void f_reverse(Finder f){
	assert(f->dir != NO_DIR);
	f->dir = r_invertdir(f->rail, f->dir);
	//f->dir = d_invert(f->dir);
}
