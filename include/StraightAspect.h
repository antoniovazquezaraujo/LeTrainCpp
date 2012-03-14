#ifndef STRAIGHTASPECT_H
#define STRAIGHTASPECT_H
#include "Basic.h"
#include "Aspect.h"
class StraightAspect : public Aspect{
public:
	StraightAspect(DirEnv * env);
	int getAspectChar();
	int getBgColor();
	int getFgColor();
protected:
	DirEnv * env;
	int aspectChar;
	enum {
		HORIZONTAL_LINE='-',
		VERTICAL_LINE='|',
		GO_UP_LINE= '/',
		GO_DOWN_LINE= '\\',
	};

	Dir getDir(DirEnv * env);
	int selectAspectChar(Dir dir);
	static Logger log;
};
#endif
