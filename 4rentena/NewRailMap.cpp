/*
  NewRailMap.cpp
  A future refactored substitute of RailMap 
  -----------------------------------------
*/
#include "NewRailMap.h"

NewRailMap::~NewRailMap(){

}
NewRailMap::NewRailMap(Charmap & charmap){
}

Rail & NewRailMap::getRail(int row, int col){
	return rails[row][col];
}

void NewRailMap::setRail(int row, int col, Rail & rail){
	rails[row][col] = rail;
}
void NewRailMap::addCross(Cross * cross){

}
List * NewRailMap::crosslist(){
	return 0;
}
int NewRailMap::load(Charmap * charmap){
	return 0;
}
void NewRailMap::setcrosses(List * crosses){

}
int NewRailMap::lookaround (Charmap * charmap, int row, int col, int directions[3][3]){
	return 0;
}
Rail * NewRailMap::newstop(Charmap * charmap, int row, int col){
	return 0;
}
Rail * NewRailMap::newcurve(Charmap * charmap, int row, int col){
	return 0;
}
int NewRailMap::getCrossDir(int directions[][3]){
	return 0;
}
int NewRailMap::crossouts (int directions[3][3], int outs[3], int keyDir){
	return 0;
}
struct railandcross NewRailMap::newCross(Charmap * charmap, int row, int col){
	struct railandcross basura;
	return basura;
}
void NewRailMap::connect(){

}

