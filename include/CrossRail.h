#ifndef CROSSRAIL_H
#define CROSSRAIL_H
#include "Basic.h"
#include "Rail.h"
class CrossRail: public Rail{
public:
	CrossRail(DirEnv * env);
	~CrossRail();
	static Logger log;
};
#endif
