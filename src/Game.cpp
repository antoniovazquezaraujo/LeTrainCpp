#include "Basic.h"
#include "Game.h"
#include "Window.h"
#include "Sim.h"
#include "SimView.h"
#include "Commander.h"
#include "CommanderView.h"
#include "TerminalWindow.h"
#include "ProgramEditor.h"
Logger Game::log = Logger::getInstance("Game");
Game::Game(){
//	int screenRows = Window::getScreenRows();
//	int screenCols = Window::getScreenCols();
//	int commandCols = screenCols * 20/100;
//	int commandRows = screenRows * 20/100;
//	int mapCols = screenCols - commandCols;
//	int mapRows = screenRows - commandRows;
//	LOG_DEBUG(log," Ventana de map" << mapCols << ", " << mapRows);
//
//	Window * simWindow = new Window(0, 0, mapCols, mapRows);
//	simWindow->setTitle("Map");
//
//	Window * commanderWindow = new Window(0, mapCols, commandCols, mapRows);
//	commanderWindow->setTitle("Commands");
//
//
//	terminalWindow = new TerminalWindow(mapRows,0, screenCols, commandRows);
//	terminalWindow->setTitle("Console");

//	sim = new Sim();
//	simView = new SimView( sim, simWindow, commanderWindow, terminalWindow); 
//
//	commander = new Commander(sim, simView); 
//	commanderView = new CommanderView(commander, commanderWindow);
	sim = new Sim(this);
	commander = new Commander(this);
	programEditor = new ProgramEditor(this);
}

Game::~Game(){
	delete sim;
	delete commander; 
	delete programEditor; 
}
Sim * Game::getSim(){
	return sim;
}
Commander * Game::getCommander(){
	return commander;
}
ProgramEditor * Game::getProgramEditor(){
	return programEditor;
}
void Game::start(){
	running = true;
	int key=-1;
	while(running){
		sim->erase();
		key = Window::getKey();
		if(key!= -1){
			//LOG4CPLUS_DEBUG(log, " pulsada tecla " << key); 
			commander->doAction(key);
		}
		sim->moveTrains();
		sim->moveWagons();
		sim->checkSensors();
		paint();
	}
}
void Game::paint(){
	sim->paint();
	commander->paint();
	programEditor->paint();
	update_panels();
	doupdate();
}
