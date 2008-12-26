#ifndef RAILENV_H
#define RAILENV_H
#include "Basic.h"
class RailEnv{
public:
	RailEnv();
	RailEnv(const RailEnv & env);
	void addPath(Dir d, Rail * r);
	Rail * getRail(Dir dir);
	friend ostream & operator << (ostream & o, RailEnv & railEnv);
private:
	Rail * links[NUM_DIRS];
	static Logger log;
};
#endif
