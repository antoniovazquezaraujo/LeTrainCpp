#define RAIL_MAP_ROWS 100 
#define RAIL_MAP_COLS 100 
#include "rail.h"
#include "charMap.h"
typedef struct railMap * RailMap;
RailMap newRailMap();
void freeRailMap(RailMap railMap);
Rail getRailMapData(RailMap railMap, int row, int col);
void setRailMapData(RailMap railMap, int row, int col, Rail rail);
int readRailMapFromCharMap(RailMap railMap, CharMap charMap);
void connectRails(RailMap railMap);
