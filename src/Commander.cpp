#include "Basic.h"
#include "Commander.h"
#include "CommanderView.h"
#include "SimView.h"
#include "Game.h"
#include "Sim.h"
#include "RailPen.h"
#include "RailMap.h"
#include "Rail.h"
#include "Finder.h"
#include "Locomotive.h"
#include "Wagon.h"
#include "Train.h"
#include "Sensor.h"
#include "Window.h"
#include "ForkRail.h"
#include "EventProgram.h"
#include "ProgramEditor.h"

MAKE_LOGGER(Commander);

extern bool parse(string s, Game * game);

Commander::Commander(Game * game)
	:game(game){

	view = new CommanderView(this);

	registerMode(DRIVERS_MODE,'D'); 
	registerMode(POINTS_MODE, 'F');
	registerMode(TRAINS_MODE, 'T');
	registerMode(RAILS_MODE,  'R');
	registerMode(RAILPEN_MODE,'P');
	registerMode(EVENTS_MODE, 'V');

	registerAction("Select next train",       &Commander::selectNextTrain,      DRIVERS_MODE, 'n');
	registerAction("Select previous train",   &Commander::selectPreviousTrain,  DRIVERS_MODE, 'p');
	registerAction("Select next vehicle",     &Commander::selectNextVehicle,    DRIVERS_MODE, 'l');
	registerAction("Select previous vehicle", &Commander::selectPreviousVehicle,DRIVERS_MODE, 'h');
	registerAction("Accelerate",              &Commander::accelerateTrain,      DRIVERS_MODE, 'k');
	registerAction("Deccelerate",             &Commander::deccelerateTrain,     DRIVERS_MODE, 'j');
	registerAction("Unlink",                  &Commander::unlinkTrain,          DRIVERS_MODE, '0');
	registerAction("Link",                    &Commander::linkTrain,            DRIVERS_MODE, '1');

	registerAction("Toggle move/paint", &Commander::toggleRailPenPainting,      RAILPEN_MODE, ' ');
	registerAction("Rotate right",      &Commander::rotateRightRailPen,         RAILPEN_MODE, 'l');
	registerAction("Rotate left",       &Commander::rotateLeftRailPen,          RAILPEN_MODE, 'h');
	registerAction("Forward",           &Commander::railPenForward,             RAILPEN_MODE, 'k');
	registerAction("Backwards",         &Commander::railPenBackwards,           RAILPEN_MODE, 'j');
	registerAction("Place finder",      &Commander::railPenPlaceFinder,         RAILPEN_MODE, 'f');
	registerAction("Load program",      &Commander::railPenLoadProgram,         RAILPEN_MODE, 'p');

	registerAction("Forward",           &Commander::finderForward,              TRAINS_MODE, 'j');
	registerAction("Reverse",           &Commander::finderReverse,              TRAINS_MODE, ' ');
	registerAction("Add wagon",         &Commander::finderAddWagon,             TRAINS_MODE, 'w');
	registerAction("Add train",         &Commander::finderAddTrain,             TRAINS_MODE, 't');
	registerAction("Add sensor",        &Commander::finderAddSensor,            TRAINS_MODE, 's');
	registerAction("Delete vehicle",    &Commander::finderDeleteVehicle,        TRAINS_MODE, 'x');
	registerAction("Inc. fork dir",     &Commander::finderIncFork,              TRAINS_MODE, 'h');
	registerAction("Dec. fork dir",     &Commander::finderDecFork,              TRAINS_MODE, 'l');
	registerAction("Set auto-move",     &Commander::setAutoMove,                TRAINS_MODE, 'a');
	registerAction("Move trains",       &Commander::moveTrains,                 TRAINS_MODE, '.');

	setMode('P');
	autoMove=true;
}
Commander::~Commander(){

}
/////////////////////////////////////////////////////////////////
void Commander::onSetDriversMode(){
}
void Commander::onSetPointsMode(){
}
void Commander::onSetTrainsMode(){
	game->getSim()->followVehicle(game->getSim()->getFinder());
}
void Commander::onSetRailsMode(){
}
void Commander::onSetRailPenMode(){
	game->getSim()->followVehicle(game->getSim()->getRailPen());
}
void Commander::onSetEventsMode(){
	game->getProgramEditor()->edit();
}
//////// Drivers actions /////////
void Commander::selectNextTrain(){
	game->getSim()->selectNextTrain();
}
void Commander::selectPreviousTrain(){
	game->getSim()->selectPrevTrain();
}
void Commander::selectNextVehicle(){
	game->getSim()->selectNextVehicle();
}
void Commander::selectPreviousVehicle(){
	game->getSim()->selectPrevVehicle();
}
void Commander::accelerateTrain(){
	game->getSim()->getSelectedTrain()->incImpulseGenerated();
}
void Commander::deccelerateTrain(){
	game->getSim()->getSelectedTrain()->decImpulseGenerated();
}
//////// RailPen actions /////////
void Commander::toggleRailPenPainting(){
	RailPen* railPen = game->getSim()->getRailPen();
	if(railPen->getMode() == RailPen::MOVING){
		railPen->setMode(RailPen::PAINTING);
	}else{
		railPen->setMode(RailPen::MOVING);
	}
}
void Commander::rotateLeftRailPen(){
	game->getSim()->getRailPen()->rotateLeft();
}
void Commander::rotateRightRailPen(){
	game->getSim()->getRailPen()->rotateRight();
}
void Commander::railPenForward(){
	RailPen* railPen = game->getSim()->getRailPen();
	Point p = railPen->getPos();
	RailMap *railMap = game->getSim()->getRailMap(); 
	if(railPen->getMode() == RailPen::PAINTING){
		Rail * newRail = railPen->makeNewRail(railMap->getRailAt(p.row, p.col));
		railMap->setRail(p.row, p.col, newRail);

		ForkRail *rf = dynamic_cast<ForkRail *>(newRail);
		if(rf){
			game->getSim()->addFork(rf);
		}

		Point lastPoint = railPen->getPos();
		Dir lastDir = railPen->getLastDir();
		Dir backDir = -lastDir; 
		lastPoint.move(backDir);
		Rail * lastRail = railMap->getRailAt(lastPoint.row, lastPoint.col);
		if(lastRail){
			newRail->linkRailAt(backDir, lastRail);
		}
	}
	railPen->forward();
}
void Commander::railPenBackwards(){
	RailPen* railPen = game->getSim()->getRailPen();
	if(railPen->getMode() == RailPen::PAINTING){
		RailMap *railMap = game->getSim()->getRailMap(); 
		Point p = railPen->getPos();
		railMap->setRail(p.row, p.col, 0);
	}
	railPen->backwards();
}
void Commander::railPenPlaceFinder(){
	RailPen* railPen = game->getSim()->getRailPen();
	Point p = railPen->getPos();
	Rail * r = game->getSim()->getRailMap()->getRailAt(p.row, p.col);
	if(r){
		game->getSim()->getFinder()->gotoRail(r);
	}
}
void Commander::railPenLoadProgram(){
	ifstream file("programa.ltr");
	if(!file){
		cout << "error no encuentro ese fichero" << endl;
		cin.get();
	}
	string sentencia;
	string linea;
	while(getline(file, linea)){
		sentencia += linea + " ";
	}
	parse(sentencia, game);
}

//////// Finder actions /////////
void Commander::finderIncFork(){
	Rail * rail = game->getSim()->getFinder()->getRail();
	ForkRail *rf = dynamic_cast<ForkRail *>(rail);
	if(rf){
		++(*rf);
	}
}
void Commander::finderDecFork(){
	Rail * rail = game->getSim()->getFinder()->getRail();
	ForkRail *rf = dynamic_cast<ForkRail *>(rail);
	if(rf){
		--(*rf);
	}
}
void Commander::finderForward(){
	if( game->getSim()->getFinder()->forward()){
	}
}
void Commander::finderReverse(){
	game->getSim()->getFinder()->reverse();
}
void Commander::unlinkTrain(){
	game->getSim()->getSelectedTrain()->unlink();
}
void Commander::linkTrain(){
	game->getSim()->getSelectedTrain()->link();
}
void Commander::finderAddWagon(){
	Train * train = new Train;
	//Wagon * w = new Wagon;
	RailVehicle* w = new Wagon;
	Finder * f = game->getSim()->getFinder();
	if(f->getRail()->getRailVehicle() == 0){
		//w->gotoRail(f->getRail());
		*(f->getRail()) < w;
		train->addVehicle(w);
		game->getSim()->addTrain(train);
		LOG_DEBUG(log," Agrego el tren en: " << f->getPos().row << "," << f->getPos().col);
	}else{
		LOG_DEBUG(log," No puedo agregar vehiculo pues hay uno");
	}
}
void Commander::finderAddTrain(){
	Train * train = new Train;
	RailVehicle * locomotive = new Locomotive;
	Finder * finder = game->getSim()->getFinder();
	if(finder->getRail()->getRailVehicle() == 0){
		//l->gotoRail(f->getRail());
		*(finder->getRail()) < locomotive;
		train->addVehicle(locomotive);
		game->getSim()->addTrain(train);
		LOG_DEBUG(log," Agrego el tren en: " << finder->getPos().row << "," << finder->getPos().col);
	}else{
		LOG_DEBUG(log," No puedo agregar vehiculo pues hay uno");
		delete train;
		delete locomotive;
	}
}
void Commander::finderAddSensor(){
	Finder * f = game->getSim()->getFinder();
	Rail * r = f->getRail();
	if(r){
		Sensor * s = new Sensor();
		s->setRail(r);
		LOG_DEBUG(log," Agregamos sensor con rail:" << s->getRail());
		game->getSim()->addSensor(s);
	}
}
void Commander::finderDeleteVehicle(){
	Finder * f = game->getSim()->getFinder();
	RailVehicle *r = f->getRail()->getRailVehicle();
	if(r){
		r->setRail(0);
		f->getRail()->setRailVehicle(0);
	}
}
void Commander::setAutoMove(){
	autoMove = !autoMove;
}

bool Commander::getAutoMove(){
	return autoMove;
}
void Commander::moveTrains(){
	game->getSim()->moveTrains();
	//game->getSim()->moveWagons();
	game->getSim()->checkSensors();
}
/////////////////////////////////////////////////////////////////
void Commander::doAction(char key){
	if(isupper(key)){
		setMode(key);
	}else{
		if(isValidAction(mode, key)){
			Method m = getAction(mode, key).method;
			(this->*m)();
		}
	}
}
void Commander::paint(){
	view->paint();
}
void Commander::erase(){
	view->erase();
}
//////////////////////////////////////////////////////////////////////
Action Commander::getAction(int mode, char key){
	return maps[mode][key];
}
bool Commander::isValidAction(int mode, char key){
	if(maps[mode].find(key) != maps[mode].end()){
		return true;
	}
	return false;
}
void Commander::registerAction(string name, Method method, int mode, char key){
	Action a;
	a.mode = mode;
	a.key = key;
	a.name = name;
	a.method = method;
	maps[mode][key] = a;
}
void Commander::registerMode(Commander::CommandMode mode, char key){
	modes[key] = mode;
	ActionsMap modeActions;
	maps[mode] = modeActions;
}
void Commander::setMode(char key){
	mode = modes[key];
	switch(mode){
	case DRIVERS_MODE: 
		onSetDriversMode();	
		break;
	case POINTS_MODE :
		onSetPointsMode();	
		break;
	case TRAINS_MODE :
		onSetTrainsMode();	
		break;
	case RAILS_MODE  :
		onSetRailsMode();	
		break;
	case RAILPEN_MODE:
		onSetRailPenMode();	
		break;
	case EVENTS_MODE:
		onSetEventsMode();	
		break;
	}
}
Commander::CommandMode Commander::getMode(){
	return mode;
}
const char * Commander::getModeName(int mode){
	const char * ret = 0;
	switch(mode){
	case DRIVERS_MODE: 
		ret = "Drivers";
		break;
	case POINTS_MODE :
		ret = "Points";
		break;
	case TRAINS_MODE :
		ret = "Trains";
		break;
	case RAILS_MODE  :
		ret = "Rails";
		break;
	case RAILPEN_MODE:
		ret = "RailPen";
		break;
	case EVENTS_MODE:
		ret = "Events";
		break;
	}
	return ret;
}
