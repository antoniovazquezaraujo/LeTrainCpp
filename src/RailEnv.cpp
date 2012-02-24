#include "Basic.h"
#include "RailEnv.h"
#include "Dir.h"
MAKE_LOGGER(RailEnv);

RailEnv::RailEnv(){
	for(int n(0);n<Dir::NUM_DIRS;n++){
		links[n] = nullptr;
	}
}
RailEnv::RailEnv(const RailEnv & env){
	this->links = env.links;
}
void RailEnv::addPath(Dir d, Rail * r){
	links[d.getValue()] = r;
}
Rail * RailEnv::getRail(Dir dir){
	return links[dir.getValue()];
}
ostream & operator << (ostream & o, RailEnv & railEnv){
	for(int n(0);n<Dir::NUM_DIRS;n++){
		if(railEnv.links[n]){
		   	o << "[" << railEnv.links[n] << "]";
		}
	}
	return o;
}

