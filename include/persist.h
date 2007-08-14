#ifndef PERSIST_H
#define PERSIST_H
#include "sim.h"
#include "stdio.h"
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
};
typedef struct data* V_data;
int loadmap(Sim * s, char * mapfilename); 
void loadgame(Sim * s, char *gamefilename);	
void savegame(Sim * s, char *gamefilename);
int invert(int dir);
int strToDir(char *c);
void dirtostr(int dir, char* dest);
void dataread(V_data data, FILE *file, int type);
void datawrite(V_data data, FILE *file);
Vehicle*  dataToVehicle(V_data data);
void vehicletodata(Vehicle*  v, V_data data);
V_data newdata();
void freedata(V_data data);
#endif
