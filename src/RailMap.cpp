#include "Basic.h"
#include "RailMap.h"
#include "Rail.h"


RailMap::RailMap() {
}
RailMap::~RailMap(){
	for(auto row : rails){
		for(auto rail: row.second){
			delete rail.second;
		}
	}
}
Rail * RailMap::getRailAt(int row, int col){
	auto i = rails.find(row);
	if(i != rails.end()){
		auto row = (*i).second;
		auto i2 = row.find(col);
		if(i2 != row.end()){
			return (*i2).second;
		}
	}
	return 0;
}
	
void RailMap::setRail(int row, int col, Rail * rail){
	if(rail != 0){
		rail->setPos(Point(row, col));
		rails[row][col] = rail;
	}else{
		rails[row].erase(col);
	}
}
void RailMap::paint(Window * g){
	for(auto row : rails){
		for(auto rail: row.second){
			rail.second->paint(g);
		}
	}
}
void RailMap::erase(Window * g){
	for(auto row : rails){
		for(auto rail: row.second){
			rail.second->erase(g);
		}
	}
}
