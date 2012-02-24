#ifndef EVENT_H
#define EVENT_H
#include "Basic.h"
class Event{
public:
	Event(int sensorId, int hour, int minute, int second, 
			int locomotiveId, int locomotiveSpeed, 
			int locomotiveDir);
	Event(const Event &);
	Event();
	int getSensorId();
	int getHour();
	int getMinute();
	int getSecond();
	int getLocomotiveId();
	int getLocomotiveSpeed();
	int getLocomotiveDir();
	void setSensorId(int n);
	void setHour(int n);
	void setMinute(int n);
	void setSecond(int n);
	void setLocomotiveId(int n);
	void setLocomotiveSpeed(int n);
	void setLocomotiveDir(int n);
	string str();
private:
	int sensorId;
	int hour;
	int minute;
	int second;
	int locomotiveId;
	int locomotiveSpeed;
	int locomotiveDir;
};
#endif
