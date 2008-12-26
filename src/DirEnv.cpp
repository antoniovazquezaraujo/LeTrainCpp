#include "Basic.h"
#include "DirEnv.h"
#include "ForkRail.h"
#include "CurveRail.h"
#include "StraightRail.h"
#include "CrossRail.h"
MAKE_LOGGER(DirEnv);

DirEnv::~DirEnv(){
}
DirEnv::DirEnv(){
	for(int n(0);n<NUM_DIRS;n++){
		dirs[n].setValue(NO_DIR);
	}
}
DirEnv::DirEnv(const DirEnv & env){
	for(int n(0);n<NUM_DIRS;n++){
		dirs[n].setValue(env.dirs[n].getValue());
	}
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
// Entrando por la direccion dir a donde se sale?
// Ojo, el que entra por el E lleva dirección W
// Por eso le damos la vuelta!
Dir DirEnv::getPath(Dir dir){
	if(dir.getValue() != NO_DIR){
//		ostringstream s;
//		s<< "con dir: " << dir.getValue() <<  " devolvemos " << dirs[dir.getValue()]<< endl;
//		const char * cad = s.str().c_str();
//		Game::debug(cad);
		return dirs[dir.getValue()];
	}else{
//		ostringstream s;
//		s<< "no hay esa dir: " << dir.getValue() << endl;
//		const char * cad = s.str().c_str();
//		Game::debug(cad);
		return Dir(NO_DIR);
	}
}
Dir DirEnv::getAnyPath(){
	for (int n(0); n<NUM_DIRS;n++){
		if (dirs[n].getValue() != NO_DIR){
			return dirs[n];
		}
	}
	LOG_DEBUG(log, " dirEnv.getAnyPath devuelve NO_DIR");
	return Dir(NO_DIR);
}
Dir DirEnv::getDir(Dir dir){
	if (dir.getValue() != NO_DIR){
		return dirs[dir.getValue()];;
	}else{
		return NO_DIR;	
	}
}

int DirEnv::getNumDirs(){
	int numDirs=0;
	for (int n(0); n<NUM_DIRS;n++){
		if(dirs[n].getValue() != NO_DIR){
			numDirs++;
		}
	}
	//cout << "Tiene " << numDirs << " dirs." <<endl;
	return numDirs;
}

bool DirEnv::isStraight(){
	//es valido si solo tiene dos dir y forman recta
	bool ret = true;
	int numDirs = 0;
	for(int n(E) ;n<(NUM_DIRS); n++){
		Dir dir (n);
		Dir found = getPath(dir);
		if(found.getValue() != NO_DIR){
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
	for(int n(E) ;n<(NUM_DIRS); n++){
		Dir dir (n);
		Dir from = getPath(dir);
		if(from.getValue() != NO_DIR){
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
	for(int n(E) ;n<(NUM_DIRS); n++){
		Dir dir (n);
		Dir from = getPath(dir);
		if(from.getValue() != NO_DIR){
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
	for(int n(E) ;n<(NUM_DIRS); n++){
		Dir dir(n);
		Dir found = getPath(dir);
		if(found.getValue() != NO_DIR){
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
		if((getPath(left)).getValue() != NO_DIR){
			return false;
		}
		if((getPath(right)).getValue() != NO_DIR){
			return false;
		}
	}
	return true;
}
ostream & operator << (ostream & o, DirEnv & env){
	int n ;
	//first row
	n= NW;
	o << "[" << setw(2) << (env.getDir(n).getValue() != NO_DIR? env.getDir(n).getName():" ") << "]";
	n= N;
	o << "[" <<setw(2) <<  (env.getDir(n).getValue() != NO_DIR? env.getDir(n).getName():" ") << "]";
	n= NE;
	o << "[" << setw(2) << (env.getDir(n).getValue() != NO_DIR? env.getDir(n).getName():" ") << "]";
	o << endl; 

	// central row
	n= W;
	o << "[" << setw(2) << (env.getDir(n).getValue() != NO_DIR? env.getDir(n).getName():" ") << "]";
	// central point
	o << "[* ]";
	n= E;
	o << "[" << setw(2) << (env.getDir(n).getValue() != NO_DIR? env.getDir(n).getName():" ") << "]";
	o << endl; 

	// third row
	n= SW;
	o << "[" << setw(2) << (env.getDir(n).getValue() != NO_DIR? env.getDir(n).getName():" ") << "]";
	n= S;
	o << "[" << setw(2) << (env.getDir(n).getValue() != NO_DIR? env.getDir(n).getName():" ") << "]";
	n= SE;
	o << "[" << setw(2) << (env.getDir(n).getValue() != NO_DIR? env.getDir(n).getName():" ") << "]";
	o << endl; 
	return o;
}
