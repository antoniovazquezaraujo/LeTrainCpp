/*
  letrain.h 
  The general declarations 
  -------------------------------------
*/
#ifndef letrain_h
#define letrain_h

#include <assert.h>
#include "logging.h"
#include "graphics.h"

#define CHAR_MAP_ROWS 500 
#define CHAR_MAP_COLS 500 
#define MAX_PAGE_ROWS 40
#define MAX_PAGE_COLS 40
#define START_ROW 10
#define START_COL 10
#define RAIL_MAP_ROWS 500 
#define RAIL_MAP_COLS 500 


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



// Direction
// int invert(int dir);
// int strToDir(char *c);
// void dirtostr(int dir, char* dest);
// void dataread(V_data data, FILE *file, int type);
// void datawrite(V_data data, FILE *file);
// Vehicle*  dataToVehicle(V_data data);
// void vehicletodata(Vehicle*  v, V_data data);
// V_data newdata();
// void freedata(V_data data);

#endif
