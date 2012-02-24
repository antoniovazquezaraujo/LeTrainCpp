#ifndef FORKRAIL_H
#define FORKRAIL_H
#include "Basic.h"
#include "Rail.h"
class ForkRail: public Rail{
public:
	ForkRail(DirEnv * env);
	~ForkRail();
	Dir getPath(Dir dir);
	void selectDir(Dir dir);
	Dir getSelectedDir();
	ForkRail & operator++();
	ForkRail & operator--();
private:
	ForkAspect * forkAspect;
	ForkDirSelector * selector;
	friend ostream & operator << (ostream & o, ForkRail & rail);
	static Logger log;
};
#endif
