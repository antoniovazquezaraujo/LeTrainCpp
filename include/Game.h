#ifndef GAME_H
#define GAME_H
#include "Basic.h"
#include "Sim.h"
#include "ProgramEditor.h"
#include "Console.h"
#include "Commander.h"
class Game{
public:
	Game();
	~Game();
	void start();
	void paint();
	Sim           sim;
	ProgramEditor programEditor;
	Console       console;
	Commander     commander;
private:
	bool running;
	static Logger log;
};
#endif
