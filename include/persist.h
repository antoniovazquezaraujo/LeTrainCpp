#ifndef PERSIST_H
#define PERSIST_H

#include <iostream>
#include <fstream>
#include <cstring>
#include "sim.h"
#include "vehicle.h"
#include "charmap.h"
#include "vehicle.h"
#include "iterator.h"
#include "sim.h"
using namespace std;
struct data{
	int id;
	int type;
	char dir[7];
	char aspect;
	int fg;
	int bg;
	int row;
	int col;
	float imp;
	int power;
	int brakes;
	int mass;
	int link0id;
	int link1id;
	ifstream entrada;
	ofstream salida;
};
typedef struct data* V_data;
int loadmap(Sim * s, char * mapfilename); 
void loadgame(Sim * s, char *gamefilename);	
void savegame(Sim * s, char *gamefilename);
int invert(int dir);
int strToDir(char *c);
void dirtostr(int dir, char* dest);
void dataread(V_data data, ifstream &file, int type);
void datawrite(V_data data, ofstream &file);
Vehicle*  dataToVehicle(V_data data);
void vehicletodata(Vehicle* v, V_data data);
V_data newdata();
void freedata(V_data data);
#endif
