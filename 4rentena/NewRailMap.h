#ifndef NEWRAILMAP_H
#define NEWRAILMAP_H

class NewRailmap;

#include "railmap.h"

class NewRailMap : public Railmap{
public:
	NewRailMap (Charmap & charmap);
	~NewRailMap ();
	int getNumRows();
	int getNumCols();
	int load(Charmap * charmap);
	void addCross(Cross * cp);
	List * crosslist();
	void setRail(int row, int col, Rail & r);
	Rail & getRail(int row, int col);
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

	Rail rails[RAIL_MAP_ROWS][RAIL_MAP_COLS];
};
#endif
