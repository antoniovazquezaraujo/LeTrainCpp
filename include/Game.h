#ifndef GAME_H
#define GAME_H
#include "Basic.h"
class Game{
public:
	Game();
	~Game();
	void start();
	void paint();
	Sim           * getSim();
	Commander     * getCommander();
	ProgramEditor * getProgramEditor();
private:
	Sim           * sim;
	Commander     * commander;
	ProgramEditor * programEditor;
	bool running;
	static Logger log;
};
#endif
