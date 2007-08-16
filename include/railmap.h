#ifndef RAILMAP_H
#define RAILMAP_H

class Railmap;
#define RAIL_MAP_ROWS  500 
#define RAIL_MAP_COLS  500 

#include "rail.h"
#include "letrain.h"
#include "list.h"
#include "cross.h"
#include "charmap.h"
#include "dir.h"

// TODO
// cambiar esta struct por dos funciones diferentes!!!!!!!!!!!!!!!!
struct railandcross{
	Rail * rail;
	Cross * cross;
};

class Railmap{
public:
	Railmap (Charmap * charmap);
	 virtual ~Railmap ()=0;
	 int load(Charmap * charmap);
	 void addCross(Cross * cp);
	 List * crosslist();
	 void setRail(int row, int col, Rail * r);
	 Rail * getRail(int row, int col);
	 Rail * rails[RAIL_MAP_ROWS][RAIL_MAP_COLS];
	 List * crosses;
protected:
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
