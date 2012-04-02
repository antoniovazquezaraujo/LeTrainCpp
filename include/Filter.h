#ifndef FILTER_H
#define FILTER_H
#include "Basic.h"
#include "Range.h"
class Filter{
public:
	Filter(Range *sensorId, Range *hour, Range *minute, Range *second, 
		Range * locomotiveId, Range *locomotiveSpeed, Range * locomotiveDir);
	Filter(const Filter & f);
	Filter();
	bool accept(Event * event);
	string str();
	Range * getSensorId();
	void setSensorId(Range * r);
	Range * getHour();
	void setHour(Range * r);
	Range * getMinute();
	void setMinute(Range * r);
	Range * getSecond();
	void setSecond(Range * r);
	Range * getLocomotiveId();
	void setLocomotiveId(Range * r);
	Range * getLocomotiveSpeed();
	void setLocomotiveSpeed(Range * r);
	Range * getLocomotiveDir();
	void setLocomotiveDir(Range * r);
private:
	Range * sensorId;
	Range * hour;
	Range * minute;
	Range * second;
	Range * locomotiveId;
	Range * locomotiveSpeed;
	Range * locomotiveDir;
	
};
#endif
