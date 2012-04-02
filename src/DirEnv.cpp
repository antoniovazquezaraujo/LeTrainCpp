#include "Basic.h"
#include "DirEnv.h"
#include "ForkRail.h"
#include "CurveRail.h"
#include "StraightRail.h"
#include "CrossRail.h"

DirEnv::~DirEnv(){
}
DirEnv::DirEnv(){
	for(auto &d: dirs){
		d = Dir::NO_DIR;
	}
	for(auto d: dirs){
		assert(d == Dir::NO_DIR);
	}
}
DirEnv::DirEnv(const DirEnv & env){
	dirs = env.dirs;
}
Rail * DirEnv::makeNewRail(){
	Rail * ret = 0;
	if(isFork()){
		ret=  new ForkRail(this);
	}else if(isCurve()){
		ret=  new CurveRail(this);
	}else if(isStraight()){
		ret=  new StraightRail(this);
	}else {
		ret=  new CrossRail(this);
	}
	return ret;
}
void DirEnv::addPath(Dir from, Dir to){
	dirs[to.getValue()] = from;
	dirs[from.getValue()] = to;
}
Dir DirEnv::getPath(Dir dir){
	if(dir.getValue() != Dir::NO_DIR){
		
		return dirs[dir.getValue()];
	}else{
		
		return Dir(Dir::NO_DIR);
	}
}
Dir DirEnv::getAnyPath(){
	for (int n(0); n<Dir::NUM_DIRS;n++){
		if (dirs[n].getValue() != Dir::NO_DIR){
			return dirs[n];
		}
	}
	
	return Dir(Dir::NO_DIR);
}
Dir DirEnv::getDir(Dir dir){
	// ????????????????????????
	return dir;
}

int DirEnv::getNumDirs(){
	int numDirs=0;
	for (int n(0); n<Dir::NUM_DIRS;n++){
		if(dirs[n].getValue() != Dir::NO_DIR){
			numDirs++;
		}
	}
	return numDirs;
}

bool DirEnv::isStraight(){
	//es valido si solo tiene dos dir y forman recta
	bool ret = true;
	int numDirs = 0;
	for(int n(Dir::E) ;n<(Dir::NUM_DIRS); n++){
		Dir dir (n);
		Dir found = getPath(dir);
		if(found.getValue() != Dir::NO_DIR){
			numDirs++;
			if(numDirs >2){
				return false;
			}
			Dir to = getPath(found);
			if(!found.isStraight(to)){
				return false;
			}
		}
	}
	return ret;
}
bool DirEnv::isCurve(){
	bool ret = true;
	int numDirs = 0;
	for(int n(Dir::E) ;n<(Dir::NUM_DIRS); n++){
		Dir dir (n);
		Dir from = getPath(dir);
		if(from.getValue() != Dir::NO_DIR){
			numDirs++;
			if(numDirs >2){
				ret = false;
				break;
			}
			Dir to = getPath(from); 
			if(!from.isCurve(to)){
				ret = false;
				break;
			}
		}
	}
	return (ret == true && numDirs == 2);
}
bool DirEnv::isCross(){
	//es valido si solo tiene rectas y tiene mas de una
	bool ret = true;
	int numDirs = 0;
	for(int n(Dir::E) ;n<(Dir::NUM_DIRS); n++){
		Dir dir (n);
		Dir from = getPath(dir);
		if(from.getValue() != Dir::NO_DIR){
			numDirs++;
			Dir to = getPath(from);
			if(!from.isStraight(to)){
				return false;
			}
		}
	}
	return (ret == true && numDirs >2);
}
bool DirEnv::isFork(){
	int numDirs = 0;
	bool haveAnInput = false;
	for(int n(Dir::E) ;n<(Dir::NUM_DIRS); n++){
		Dir dir(n);
		Dir found = getPath(dir);
		if(found.getValue() != Dir::NO_DIR){
			numDirs++;
			if(canBeAForkInput(found)){
				haveAnInput = true;
			}
		}
	}
	return ((numDirs > 2) && (haveAnInput));
}
bool DirEnv::canBeAForkInput(Dir & dir){
	Dir left = dir; 
	Dir right = dir;
	for(int n(0); n< 2; n++){
		left--;
		right++;
		if((getPath(left)).getValue() != Dir::NO_DIR){
			return false;
		}
		if((getPath(right)).getValue() != Dir::NO_DIR){
			return false;
		}
	}
	return true;
}
ostream & operator << (ostream & o, DirEnv & env){
	int n ;
	//first row
	n= Dir::NW;
	o << "[" << setw(2) << (env.getDir(n).getValue() != Dir::NO_DIR? env.getDir(n).getName():" ") << "]";
	n= Dir::N;
	o << "[" << setw(2) << (env.getDir(n).getValue() != Dir::NO_DIR? env.getDir(n).getName():" ") << "]";
	n= Dir::NE;
	o << "[" << setw(2) << (env.getDir(n).getValue() != Dir::NO_DIR? env.getDir(n).getName():" ") << "]";
	o << endl; 

	// central row
	n= Dir::W;
	o << "[" << setw(2) << (env.getDir(n).getValue() != Dir::NO_DIR? env.getDir(n).getName():" ") << "]";
	// central point
	o << "[* ]";
	n= Dir::E;
	o << "[" << setw(2) << (env.getDir(n).getValue() != Dir::NO_DIR? env.getDir(n).getName():" ") << "]";
	o << endl; 

	// third row
	n= Dir::SW;
	o << "[" << setw(2) << (env.getDir(n).getValue() != Dir::NO_DIR? env.getDir(n).getName():" ") << "]";
	n= Dir::S;
	o << "[" << setw(2) << (env.getDir(n).getValue() != Dir::NO_DIR? env.getDir(n).getName():" ") << "]";
	n= Dir::SE;
	o << "[" << setw(2) << (env.getDir(n).getValue() != Dir::NO_DIR? env.getDir(n).getName():" ") << "]";
	o << endl; 
	return o;
}
