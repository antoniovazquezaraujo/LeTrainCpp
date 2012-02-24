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
Game::Game()
	:programEditor(this),
	commander(this){
}

Game::~Game(){
}
void Game::start(){
	running = true;
	int key=-1;
	while(running){
		sim.erase();
		key = Window::getKey();
		if(key!= -1){
			commander.doAction(key);
		}
		if(commander.getAutoMove()){
			sim.moveTrains();
			sim.checkSensors();
		}
		paint();
	}
}
void Game::paint(){
	sim          .paint();
	commander    .paint();
	programEditor.paint();
	update_panels();
	doupdate();
}
