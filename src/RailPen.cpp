#include "Basic.h"
#include "RailPen.h"
#include "Window.h"
#include "DirEnv.h"
#include "Rail.h"
#include "Sim.h"
#include "RailMap.h"
#include "ForkRail.h"
MAKE_LOGGER(RailPen);

void RailPen::paint(Window * g){
	g->setBg(0);
	g->setFg(7);
	Point p = pos;
	if(mode==MOVING){
		g->putC(p.row, p.col, '+');
	}else{
		g->putC(p.row, p.col, '*');
	}
	p.move(dir);
	g->putC(p.row, p.col, dir.getAspectChar());
}
void RailPen::erase(Window * g){
	g->setBg(0);
	g->setFg(7);
	Point p = pos;
	g->putC(p.row, p.col, ' ');
	p.move(dir);
	g->putC(p.row, p.col, ' ');
}

RailPen::RailPen(Dir dir)
	:Vehicle(0),lastDir(dir), mode(MOVING){
	setDir(dir);
}

RailPen::~RailPen(){

}
Rail * RailPen::makeNewRail(Rail * rail){
	DirEnv env;
	if(rail){
		env = rail->getEnv();
	}
	LOG4CPLUS_DEBUG(log, " agregando dir y -dir:" << dir.getValue()<< ", " << (-lastDir).getValue()); 
	env.addPath(-lastDir, dir);
	Rail * newRail = env.makeNewRail();
	if(rail){
		for(int n(Dir::E) ;n<(Dir::NUM_DIRS); n++){
			Dir dir (n);
			Rail * linked = rail->getLinkedRailAt(dir);
			if(linked){
				newRail->linkRailAt(dir, linked);
			}
		}
	}
	return newRail;	
}
Dir RailPen::getLastDir(){
	return lastDir;
}
// ojo, no pinta cuando distance es mayor que 1. Testear esto!!!!!!!!!!!!!!
bool RailPen::move(int distance, Sim * sim){
	Point p = getPos();
	if(getMode() == PAINTING){
		if(distance>0){
			for(int n= 0; n<distance;n++){
				Rail * newRail = makeNewRail(sim->railMap.getRailAt(p.row, p.col));
				sim->railMap.setRail(p.row, p.col, newRail);

				ForkRail *rf = dynamic_cast<ForkRail *>(newRail);
				if(rf){
					sim->addFork(rf);
				}

				Point lastPoint = getPos();
				Dir lastDir = getLastDir();
				Dir backDir = -lastDir; 
				lastPoint.move(backDir);
				Rail * lastRail = sim->railMap.getRailAt(lastPoint.row, lastPoint.col);
				if(lastRail){
					newRail->linkRailAt(backDir, lastRail);
				}
			 	forward();	
			}
		}else{
			for(int n=0; n>distance;n--){
				Point p = getPos();
				sim->railMap.setRail(p.row, p.col, 0);
				backwards();
			}
		}
	}else{
		pos.move(dir, distance);
		lastDir=dir;
	}
	return true;
}
bool RailPen::forward(int distance){
	pos.move(dir, distance);
	lastDir=dir;
	return true;
}
bool RailPen::backwards(int distance){
	pos.move(dir+Dir::MIDDLE_ANGLE, distance);
	return true;
}

void RailPen::setMode(RailPenMode mode){
	this->mode = mode;
}
RailPen::RailPenMode RailPen::getMode(){
	return mode;
}

ostream & operator << (ostream & o, const RailPen & pen){
	int d = pen.getDir().getValue();
	int n ;
	//first row
	n= Dir::NW;
	o << (d==n?"o":" ");
	n= Dir::N;
	o << (d==n?"o":" ");
	n= Dir::NE;
	o << (d==n?"o":" ");
	o << endl; 

	// central row
	n= Dir::W;
	o << (d==n?"o":" "); 
	// central point
	o << pen.getDir(); 
	n= Dir::E;
	o << (d==n?"o":" ");
	o << endl; 

	// third row
	n= Dir::SW;
	o << (d==n?"o":" ");
	n= Dir::S;
	o << (d==n?"o":" ");
	n= Dir::SE;
	o << (d==n?"o":" ");
	o << endl; 
	return o;
}
