#include "Basic.h"
#include "RailMap.h"
#include "Rail.h"
MAKE_LOGGER(RailMap);

RailMap::RailMap() {
}
RailMap::~RailMap(){
}
Rail * RailMap::getRailAt(int row, int col){
//	return rails[row][col];
	RailGrid::iterator i = rails.find(row);
	if(i != rails.end()){
		RailMapRow row = (*i).second;
		RailMapRow::iterator i2 = row.find(col);
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
	RailGrid::iterator i = rails.begin();
	while(i != rails.end()){
		RailMapRow row = (*i).second;
		RailMapRow::iterator i2 = row.begin();
		while(i2 != row.end()){
			(*i2).second->paint(g);
			i2++;
		}
		i++;
	}
}
void RailMap::erase(Window * g){
	RailGrid::iterator i = rails.begin();
	while(i != rails.end()){
		RailMapRow row = (*i).second;
		RailMapRow::iterator i2 = row.begin();
		while(i2 != row.end()){
			(*i2).second->erase(g);
			i2++;
		}
		i++;
	}
}
