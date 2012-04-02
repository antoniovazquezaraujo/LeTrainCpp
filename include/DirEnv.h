#ifndef DIRENV_H
#define DIRENV_H
#include "Basic.h"
#include "Dir.h"
class DirEnv{
public:
	DirEnv();
	~DirEnv();
	DirEnv(const DirEnv & env);
	Rail * makeNewRail();
	void addPath(Dir from, Dir to);
	Dir getDir(Dir dir);
	Dir getPath(Dir dir);
	Dir getAnyPath();
	bool canBeAForkInput(Dir &dir);
private:
	friend ostream & operator << (ostream & o, DirEnv & env);
	array<Dir,Dir::NUM_DIRS>dirs;
	int getNumDirs();
	bool isStraight();
	bool isCurve();
	bool isCross();
	bool isFork();
	
};
#endif
