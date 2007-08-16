#ifndef CROSS_H
#define CROSS_H

class Cross;
#include "rail.h"
#include "dir.h"

class Cross{
public:
	Cross(Rail * r, int keydir, int outs[]);
	~Cross();
	void setDir(int index);
	void nextDir();
	void updateRail();
	int getDir();
	int keyDir();
	Rail * getRail();

	Rail * rail;
	int in;
	int outs[3];
	int out;
};
#endif
