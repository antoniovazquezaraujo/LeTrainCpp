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
	Dir dirs[NUM_DIRS];
	int getNumDirs();
	bool isStraight();
	bool isCurve();
	bool isCross();
	bool isFork();
	static Logger log;
};
#endif
