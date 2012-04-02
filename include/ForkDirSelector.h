#ifndef FORKDIRSELECTOR_H
#define FORKDIRSELECTOR_H
#include "Basic.h"
#include "Dir.h"
class ForkDirSelector{
public:
	ForkDirSelector(DirEnv * env);
	void selectDir(Dir dir);
	Dir getSelectedDir();
	void setNextDir();
	void setPrevDir();
	Dir getPath(Dir dir);
private:
	DirEnv * env;
	array<Dir, 3> outs;
	Dir in;	
	int selectedOut;
	int numOuts;
	friend ostream & operator << (ostream & o, ForkDirSelector & selector);
	
};
#endif
