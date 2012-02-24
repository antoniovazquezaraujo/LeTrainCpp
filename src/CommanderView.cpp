#include "Basic.h"
#include "CommanderView.h"
#include "Window.h"
#include "Commander.h"
MAKE_LOGGER(CommanderView);

CommanderView::CommanderView(Commander * commander )
	:commander(commander){
	int screenRows = Window::getScreenRows();
	int screenCols = Window::getScreenCols();
	int commandCols = screenCols * 20/100;
	int commandRows = screenRows * 20/100;
	int mapCols = screenCols - commandCols;
	int mapRows = screenRows - commandRows;

	window= new Window(0, mapCols, commandCols, mapRows);
	window->setTitle("Commands");
}
CommanderView::~CommanderView(){
	delete window; 
}
void CommanderView::erase(){
	window->erase();
}
void CommanderView::paint(){
	window->setBg(0);
	window->setFg(7);
	erase();
	int row = 0;
	Commander::ModesMap::iterator im = commander->modes.begin();
	while (im != commander->modes.end()){
		char c = (*im).first;
		Commander::CommandMode m = (*im).second;
		const char * name = commander->getModeName(m);
		window->putC(row, 0, '\'');
		window->putC(row, 1, c);
		window->putC(row, 2, '\'');
		window->putStr(row, 4, name);
		row++;
		im++;
	}
	window->putStr(row, 5, "-------------");
	row++;
	Commander::ActionsMap::iterator i = commander->maps[commander->mode].begin();
	window->putStr(row, 0, commander->getModeName(commander->mode));
	row++;
	while (i != commander->maps[commander->mode].end()){
		Action a = (*i).second;
		window->putC(row, 0, '\'');
		window->putC(row, 1, a.key);
		window->putC(row, 2, '\'');
		window->putStr(row, 4, a.name.c_str());
		row++;
		i++;
	}
	window->repaint();

}
