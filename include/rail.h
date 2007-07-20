#include "point.h"
enum{
	STOP_RAIL, 
	VERTICAL_RAIL,
	HORIZONTAL_RAIL,
	DOWN_DIAGONAL_RAIL,
	UP_DIAGONAL_RAIL,
	CURVE_RAIL,
	CROSS_RAIL
};
typedef struct rail *Rail;

Rail newRail(int type, int row, int col);
void freeRail(Rail  rail);
void addRailLink(Rail rail, int dir, Rail linked);
Rail getRailLink(Rail rail, int dir);
void addRailDir(Rail rail, int dir, int value);
int getRailDir(Rail rail, int dir);
Point getRailPosition(Rail rail);
int getRailType(Rail rail);
void defaultRailAspect(Rail rail);
