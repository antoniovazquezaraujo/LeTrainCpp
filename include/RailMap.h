#ifndef RAILMAP_H
#define RAILMAP_H
#include "Basic.h"

typedef map<int, Rail*> RailMapRow;
typedef map < int, RailMapRow> RailGrid;
class RailMap{
public:
	RailMap ();
	~RailMap ();
	void setRail(int row, int col, Rail * r);
	Rail * getRailAt(int row, int col);
	void paint(Window * g);
	void erase(Window * g);
private:
	RailGrid  rails;
	static Logger log;
};
#endif
