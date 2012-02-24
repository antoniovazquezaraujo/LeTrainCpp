#ifndef CURVERAIL_H
#define CURVERAIL_H
#include "Basic.h"
#include "Rail.h"
class CurveRail: public Rail{
public:
	CurveRail(DirEnv * env);
	~CurveRail();
	static Logger log;
};
#endif
