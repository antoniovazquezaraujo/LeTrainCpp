#ifndef GAME_H
#define GAME_H
#include "Basic.h"
class Game{
public:
	Game();
	~Game();
	void start();
	Sim * getSim();
	Commander * getCommander();
	ProgramEditor * getProgramEditor();
	void paint();
private:
	bool running;
	Sim * sim;
	Commander * commander;
	ProgramEditor * programEditor;
	static Logger log;
};
#endif
