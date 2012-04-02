#include "Basic.h"
#include "ProgramEditor.h"
#include "Dir.h"
#include "Sim.h"
#include "Game.h"

extern bool parse(string s, Game * game);
ProgramEditor::ProgramEditor (Game * game)
	:game(game){
	int screenRows = Window::getScreenRows();
	int screenCols = Window::getScreenCols();
	int commandRows = screenRows * 20/100;
	int mapRows = screenRows - commandRows;

	window = new TerminalWindow(mapRows,0, screenCols, commandRows, true);
	window->setTitle("Console");
}
TerminalWindow * ProgramEditor::getWindow(){
	return window;
}
void ProgramEditor::edit(){
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
void ProgramEditor::paint(){
	window->repaint();
}
