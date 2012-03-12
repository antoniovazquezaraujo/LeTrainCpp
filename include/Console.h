#ifndef CONSOLE_H
#define CONSOLE_H
#include "Basic.h"
#include "Sim.h"
#include "EventProgram.h"
#include "TerminalWindow.h"
#include "Message.h"

class Console{
public:
	Console (Game *game);
	void edit();
	void add(string);
	void paint();
	TerminalWindow * getWindow();
private:
	TerminalWindow * window ;
	Game * game;
	static Logger log;
};
#endif
