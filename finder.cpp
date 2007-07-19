/*
  finder.c
  The rail pointer 
  -------------------------------------
*/
#include "letrain.h"

Finder::Finder(Vehicle * v){
	pos.row=0;
	pos.col=0;
	fg = 4;
	bg = 0;
	if(v != NULL){
		rail = v->rail;
		dir = v->dir;
		pos.row= v->pos.row;
		pos.col= v->pos.col;
	}else{
		rail = NULL;
		dir = NO_DIR;
	}
}
Finder::~Finder(){

}
void Finder::gotoRail(Rail * r){
	int inputDir = NO_DIR;
	if(dir != NO_DIR){
		inputDir = invert(dir);
	}else{
		inputDir = r->validDir();
	}
	dir = r->getDir(inputDir);
	pos.row = r->pos.row;
	pos.col = r->pos.col;
	rail = r;
}
int Finder::forward(){
	int ret = 0;
	Rail * next = rail->link(dir);
	if( next != NULL){
		gotoRail(next);
		ret = 1;
	}else{
		//lg("forward",1, "finder intentando salirse por %d en %d, %d", f->dir, f->pos.row, f->pos.col);
	}
	return ret;
}
void Finder::reverse(){
	//assert(f->dir != NO_DIR);
	dir = rail->invertDir(dir);
	//f->dir = invert(f->dir);
}
