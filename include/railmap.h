#ifndef RAILMAP_H
#define RAILMAP_H

#include "rail.h"
#include "letrain.h"
#include "list.h"
#include "cross.h"
#include "charmap.h"

class Vehicle;
class Finder;
class Point;
class Commander;
class Rail;
class Cross;
class Charmap;
class Railmap;
class Sim;
class Node;
class List;
class Iterator;

class Railmap{
public:
	Railmap (Charmap * charmap);
	~Railmap ();
	int load(Charmap * charmap);
	void addCross(Cross * cp);
	List * crosslist();
	void setRail(int row, int col, Rail * r);
	Rail * getRail(int row, int col);
	Rail * rails[RAIL_MAP_ROWS][RAIL_MAP_COLS];
	List * crosses;
private:
	int lookaround (Charmap * charmap, int row, int col, int directions[][3]);
	Rail * newcurve(Charmap * charmap, int row, int col);
	struct railandcross newCross(Charmap * charmap, int row, int col);
	Rail * newstop(Charmap * charmap, int row, int col);
	int crossouts (int directions[][3], int outs[3], int keyDir);
	void connect();
	void setcrosses(List * crosses);
	int getCrossDir(int directions[][3]);
};
#endif
