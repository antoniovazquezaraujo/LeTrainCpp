#ifndef PROGRAMEDITOR_H
#define PROGRAMEDITOR_H
#include "Basic.h"
#include "Sim.h"
#include "EventProgram.h"
#include "TerminalWindow.h"
#include "Message.h"
#include "ProgramEditor.h"

class ProgramEditor{
public:
	ProgramEditor (Game *game);
	void edit();
	void paint();
	TerminalWindow * getWindow();
private:
	TerminalWindow * window ;
	Game * game;
	static Logger log;
};
#endif
