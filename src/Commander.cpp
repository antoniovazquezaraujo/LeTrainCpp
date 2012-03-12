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
	registerMode(LINK_MODE,   'L');

	registerAction("Next train",       &Commander::selectNextTrain,            DRIVERS_MODE, 'n');
	registerAction("Prev train",       &Commander::selectPrevTrain,            DRIVERS_MODE, 'p');
	registerAction("Next vehicle",     &Commander::selectNextVehicle,          DRIVERS_MODE, 'l');
	registerAction("Prev vehicle",     &Commander::selectPrevVehicle,          DRIVERS_MODE, 'h');
	registerAction("Accelerate",       &Commander::accelerateTrain,            DRIVERS_MODE, 'k');
	registerAction("Deccelerate",      &Commander::deccelerateTrain,           DRIVERS_MODE, 'j');

	registerAction("Move/paint",       &Commander::toggleRailPenPainting,      RAILPEN_MODE, ' ');
	registerAction("Rotate right",     &Commander::rotateRightRailPen,         RAILPEN_MODE, 'l');
	registerAction("Rotate left",      &Commander::rotateLeftRailPen,          RAILPEN_MODE, 'h');
	registerAction("Forward",          &Commander::railPenForward,             RAILPEN_MODE, 'k');
	registerAction("Backwards",        &Commander::railPenBackwards,           RAILPEN_MODE, 'j');
	registerAction("Place finder",     &Commander::railPenPlaceFinder,         RAILPEN_MODE, 'f');
	registerAction("Load program",     &Commander::railPenLoadProgram,         RAILPEN_MODE, 'p');

	registerAction("Forward",          &Commander::finderForward,              TRAINS_MODE, 'j');
	registerAction("Reverse",          &Commander::finderReverse,              TRAINS_MODE, ' ');
	registerAction("Add wagon",        &Commander::finderAddWagon,             TRAINS_MODE, 'w');
	registerAction("Add train",        &Commander::finderAddTrain,             TRAINS_MODE, 't');
	registerAction("Add sensor",       &Commander::finderAddSensor,            TRAINS_MODE, 's');
	registerAction("Del. vehicle",     &Commander::finderDeleteVehicle,        TRAINS_MODE, 'x');
	registerAction("Inc. fork dir",    &Commander::finderIncFork,              TRAINS_MODE, 'h');
	registerAction("Dec. fork dir",    &Commander::finderDecFork,              TRAINS_MODE, 'l');
	registerAction("Auto-move",        &Commander::setAutoMove,                TRAINS_MODE, 'a');
	registerAction("Move trains",      &Commander::moveTrains,                 TRAINS_MODE, '.');

	registerAction("Next train",       &Commander::selectNextTrain,            LINK_MODE,   'n');
	registerAction("Prev train",       &Commander::selectPrevTrain,            LINK_MODE,   'p');
	registerAction("Next vehicle",     &Commander::selectNextVehicle,          LINK_MODE,   'k');
	registerAction("Prev vehicle",     &Commander::selectPrevVehicle,          LINK_MODE,   'j');
	registerAction("Link/Unlink",      &Commander::toggleLink,                 LINK_MODE,   ' ');

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
	game->sim.followVehicle(game->sim.getFinder());
}
void Commander::onSetRailsMode(){
}
void Commander::onSetRailPenMode(){
	game->sim.followVehicle(&(game->sim.railPen));
}
void Commander::onSetEventsMode(){
	game->programEditor.edit();
}
void Commander::onSetLinkMode(){

}
//////// Drivers actions /////////
void Commander::selectNextTrain(){
	game->sim.selectNextTrain();
}
void Commander::selectPrevTrain(){
	game->sim.selectPrevTrain();
}
void Commander::selectNextVehicle(){
	game->sim.selectNextVehicle();
}
void Commander::selectPrevVehicle(){
	game->sim.selectPrevVehicle();
}
void Commander::accelerateTrain(){
	Sim::TTrains::iterator i = game->sim.getSelectedTrain();
	(*i)->incImpulseGenerated();


//	(*(game->sim.getSelectedTrain()))->incImpulseGenerated();
}
void Commander::deccelerateTrain(){
	(*(game->sim.getSelectedTrain()))->decImpulseGenerated();
}
//////// RailPen actions /////////
void Commander::toggleRailPenPainting(){
	if(game->sim.railPen.getMode() == RailPen::MOVING){
		game->sim.railPen.setMode(RailPen::PAINTING);
	}else{
		game->sim.railPen.setMode(RailPen::MOVING);
	}
}
void Commander::rotateLeftRailPen(){
	game->sim.railPen.rotateLeft();
}
void Commander::rotateRightRailPen(){
	game->sim.railPen.rotateRight();
}
void Commander::railPenForward(){
	Point p = game->sim.railPen.getPos();
	if(game->sim.railPen.getMode() == RailPen::PAINTING){
		Rail * newRail = game->sim.railPen.makeNewRail(game->sim.railMap.getRailAt(p.row, p.col));
		game->sim.railMap.setRail(p.row, p.col, newRail);

		ForkRail *rf = dynamic_cast<ForkRail *>(newRail);
		if(rf){
			game->sim.addFork(rf);
		}

		Point lastPoint = game->sim.railPen.getPos();
		Dir lastDir = game->sim.railPen.getLastDir();
		Dir backDir = -lastDir; 
		lastPoint.move(backDir);
		Rail * lastRail = game->sim.railMap.getRailAt(lastPoint.row, lastPoint.col);
		if(lastRail){
			newRail->linkRailAt(backDir, lastRail);
		}
	}
	game->sim.railPen.forward();
}
void Commander::railPenBackwards(){
	if(game->sim.railPen.getMode() == RailPen::PAINTING){
		Point p = game->sim.railPen.getPos();
		game->sim.railMap.setRail(p.row, p.col, 0);
	}
	game->sim.railPen.backwards();
}
void Commander::railPenPlaceFinder(){
	Point p = game->sim.railPen.getPos();
	Rail * r = game->sim.railMap.getRailAt(p.row, p.col);
	if(r){
		game->sim.getFinder()->gotoRail(r);
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
	Rail * rail = game->sim.getFinder()->getRail();
	ForkRail *rf = dynamic_cast<ForkRail *>(rail);
	if(rf){
		++(*rf);
	}
}
void Commander::finderDecFork(){
	Rail * rail = game->sim.getFinder()->getRail();
	ForkRail *rf = dynamic_cast<ForkRail *>(rail);
	if(rf){
		--(*rf);
	}
}
void Commander::finderForward(){
	game->sim.getFinder()->forward();
}
void Commander::finderReverse(){
	game->sim.getFinder()->reverse();
}
void Commander::finderAddWagon(){
	Train * train = new Train;
	RailVehicle* wagon = new Wagon;
	Finder * f = game->sim.getFinder();
	if(f->getRail()->getRailVehicle() == 0){
		f->getRail()->enter(wagon);
		train->addVehicle(0, wagon);
		game->sim.addTrain(train);
		LOG_DEBUG(log," Agrego el tren en: " << f->getPos().row << "," << f->getPos().col);
	}else{
		LOG_DEBUG(log," No puedo agregar vehiculo pues hay uno");
		delete wagon;
	}
}
void Commander::finderAddTrain(){
	Train * train = new Train;
	RailVehicle * locomotive = new Locomotive;
	Finder * finder = game->sim.getFinder();
	if(finder->isEmpty()){
		//l->gotoRail(f->getRail());
		finder->getRail()->enter(locomotive);
		train->addVehicle(0, locomotive);
		game->sim.addTrain(train);
		LOG_DEBUG(log," Agrego el tren en: " << finder->getPos().row << "," << finder->getPos().col);
	}else{
		LOG_DEBUG(log," No puedo agregar vehiculo pues hay uno");
		delete train;
		delete locomotive;
	}
}
void Commander::finderAddSensor(){
	Finder * f = game->sim.getFinder();
	Rail * r = f->getRail();
	if(r){
		Sensor * s = new Sensor();
		s->setRail(r);
		LOG_DEBUG(log," Agregamos sensor con rail:" << s->getRail());
		game->sim.addSensor(s);
	}
}
void Commander::finderDeleteVehicle(){
	Finder * f = game->sim.getFinder();
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
	game->sim.moveTrains();
	//game->sim.moveWagons();
	game->sim.checkSensors();
}
/////////////////////////////////////////////////////////////////
void Commander::toggleLink(){
	game->sim.toggleLink();
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
	case LINK_MODE:
		onSetLinkMode();	
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
	case LINK_MODE:
		ret = "Links";
		break;
	}
	return ret;
}
