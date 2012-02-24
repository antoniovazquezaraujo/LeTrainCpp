#ifndef STRAIGHTRAIL_H
#define STRAIGHTRAIL_H
#include "Basic.h"
#include "StraightRail.h"
#include "Rail.h"
class StraightRail: public Rail{
public:
	StraightRail(DirEnv * env);
	~StraightRail();
	static Logger log;
};
#endif
