#ifndef RAILMAP_H
#define RAILMAP_H
#include "Basic.h"

class RailMap{
public:
	RailMap ();
	~RailMap ();
	void setRail(int row, int col, Rail * r);
	Rail * getRailAt(int row, int col);
	void paint(Window * g);
	void erase(Window * g);
private:
	map<int, map<int, Rail*>> rails;
	static Logger log;
};
#endif
