#ifndef RAILENV_H
#define RAILENV_H
#include "Basic.h"
#include "Dir.h"
class RailEnv{
public:
	RailEnv();
	RailEnv(const RailEnv & env);
	void addPath(Dir d, Rail * r);
	Rail * getRail(Dir dir);
	friend ostream & operator << (ostream & o, RailEnv & railEnv);
	array<Rail *,Dir::NUM_DIRS> links;
	
};
#endif
