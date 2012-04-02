#ifndef COMMANDERVIEW_H
#define COMMANDERVIEW_H

#include "Basic.h"
class CommanderView{
public:
	CommanderView (Commander* commander);
	~CommanderView();
	void paint();
	void erase();
private:
	Commander * commander;
	Window * window ;
	
};
#endif
