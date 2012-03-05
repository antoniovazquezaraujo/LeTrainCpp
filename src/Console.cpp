#include "Basic.h"
#include "Console.h"
#include "Dir.h"
#include "Sim.h"
#include "Game.h"
MAKE_LOGGER(Console);
extern bool parse(string s, Game * game);
Console::Console (Game * game)
	:game(game){
	int screenRows = Window::getScreenRows();
	int screenCols = Window::getScreenCols();
	int commandRows = screenRows * 20/100;
	int mapRows = screenRows - commandRows;

	window = new TerminalWindow(mapRows,0, screenCols, commandRows, true);
	window->setTitle("Console");
}
TerminalWindow * Console::getWindow(){
	return window;
}
void Console::add(string info){
	(*window) << info;
}
void Console::edit(){
	string sentencia;
	window->show(true);
	(*window) << ">";
	string linea;
	while(true){
		(*window) >> linea;
		if(linea != ""){
			sentencia += " ";
			sentencia += linea;
		}else{
			parse(sentencia, game);
			break;
		}
	}
	window->show(false);
}
void Console::paint(){
	window->repaint();
}
