#ifndef OLDRAILMAP_H
#define OLDRAILMAP_H

//class OldRailmap;
//class Railmap;
#include "railmap.h"

class OldRailmap : public Railmap{
public:
	OldRailmap ();
	OldRailmap (Charmap * charmap);
	~OldRailmap ();
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
