#ifndef SENSOR_H
#define SENSOR_H
#include "Basic.h"
#include "Point.h"
class Sensor{
public:
	Sensor(int id=-1);
	Event * check();
	bool isArmed();
	void setArmed(bool armed=true);
	Rail * getRail();
	void setRail(Rail * r);
	void paint(Window * w);
	void erase(Window * w);
	int getId();
	void setId(int id);
	Point getPos();
	void setRow(int row);
	void setCol(int col);
private:
	int id;
	bool armed;
	Rail * rail;
	Point pos;
	static Logger log;
};
#endif
