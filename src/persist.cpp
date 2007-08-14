/*
  persist.cpp
  The persistence manager 
  -------------------------------------
*/
#include <string.h>
#include <stdio.h>
#include "persist.h"
#include "charmap.h"
#include "vehicle.h"
#include "iterator.h"
#include "sim.h"
int loadmap(Sim * s, char * filename){ 
	FILE * mapfile = fopen(filename, "r");
	Charmap * cm = new Charmap();
	int numRow= 0; 
	int numCol= 0; 
	int maxcols = 0;
	char *line = (char*)malloc(CHAR_MAP_COLS); 
	assert( mapfile!= 0);
	while(fgets(line, CHAR_MAP_COLS, mapfile) != 0){
		maxcols = strlen(line);
		if(numCol > maxcols) {
			maxcols = numCol;
		}
		for(numCol = 0; line[numCol] != '\n'; numCol++){
			cm->h_setchar(numRow,numCol, line[numCol]);
		}
		numRow++;
	}
	free(line);
	s->railmap = new Railmap(cm);
	free(cm);
	s->i_crosses = s->railmap->crosses->iterator();
	s->maprows = numRow;
	s->mapcols=  maxcols;
	return numRow;
}
void loadgame(Sim * s, char * filename){	
	FILE * gamefile = fopen(filename, "r");
	V_data data = newdata();
	Vehicle*  v = 0;
	int n=0;
	char section[40];
	if(gamefile != 0){
		while(1){
			n = fscanf(gamefile, "%s",section);
			if(n == EOF){
				break;
			}
			if(!strcmp(section, "locomotive")){
				dataread(data, gamefile, LOCOMOTIVE);
				v = dataToVehicle(data);
				s->addVehicle(v);
			}else if(!strcmp(section, "wagon")){
				dataread(data, gamefile, WAGON);
				v = dataToVehicle(data);
				s->addVehicle(v);
			}else{
				printf("error\n");
			}
		}
	}
	freedata(data);
}
void savegame(Sim * s, char * filename){
	FILE * gamefile = fopen(filename, "w");
	V_data data = newdata();
	Vehicle*  v = 0;
	Iterator * i = s->vehicles->iterator();
	while(i->hasNext()){
		v = (Vehicle* )i->next();
		vehicletodata(v, data);
		datawrite(data, gamefile);
	}
	delete i;
	freedata(data);
}

 void dataread(V_data data, FILE *file, int type){
	char dummy[20];
	int id;
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

	fscanf(file, "%s %d %s %s %s %c %s %d %s %d %s %d %s %d %s %f %s %d %s %d %s %d %s %d %s %d", 
			dummy, 
			&id, 
			dummy, 
			dir, 
			dummy, 
			&aspect, 
			dummy, 
			&fg, 
			dummy, 
			&bg, 
			dummy, 
			&row, 
			dummy, 
			&col, 
			dummy, 
			&imp,
			dummy, 
			&power, 
			dummy, 
			&brakes, 
			dummy, 
			&mass, 
			dummy, 
			&link0id, 
			dummy, 
			&link1id); 

	data->id = id;
	data->type = type;
	strcpy(data->dir,dir);
	data->aspect = aspect;
	data->fg = fg;
	data->bg = bg;
	data->row = row;
	data->col = col;
	data->imp = imp;
	data->power = power;
	data->brakes = brakes;
	data->mass = mass;
	data->link0id = link0id;
	data->link1id = link1id;
}
 void datawrite(V_data data, FILE *file){
	fprintf(file, "%s\n id %d\n dir %s\n aspect %c\n fg %d\n bg %d\n row %d\n col %d\n imp %f\n power %d\n brakes %d\n mass %d\n link0id %d\n link1id %d\n\n",
	data->type==LOCOMOTIVE?"locomotive":"wagon",
	data->id,
	data->dir,
	data->aspect,
	data->fg,
	data->bg,
	data->row,
	data->col,
	data->imp,
	data->power,
	data->brakes,
	data->mass,
	data->link0id,
	data->link1id);
}

 Vehicle*  dataToVehicle(V_data data){
	Vehicle*  v = new Vehicle (data->type);
	v->id = data->id;
	v->dir = strToDir(data->dir);
	v->aspect = data->aspect;
	v->fg = data->fg;
	v->bg = data->bg;
	v->pos.row = data->row;
	v->pos.col = data->col;
	v->imp = data->imp;
	v->power = data->power;
	v->brakes = data->brakes;
	v->mass = data->mass;
	v->link0id = data->link0id;
	v->link1id = data->link1id;
	return v;
}
 void vehicletodata(Vehicle*  v, V_data data){
	data->id = v->id;
	data->type = v->type;
	//strcpy(data->dir, dirtostr(v->dir));
	char ret[7];
	dirtostr(v->dir, ret);
	strcpy(data->dir, ret);
	data->aspect = v->aspect;
	data->fg = v->fg;
	data->bg = v->bg;
	data->row = v->pos.row;
	data->col = v->pos.col;
	data->imp = v->imp;
	data->power = v->power;
	data->brakes = v->brakes;
	data->mass = v->mass;
	data->link0id = v->links[0]!= 0?v->links[0]->id:0;
	data->link1id = v->links[1]!= 0?v->links[1]->id:0;
}

 V_data newdata(){
	V_data data = (V_data)malloc(sizeof(struct data));
	data->id = 0 ;
	data->type = WAGON;
	strcpy(data->dir,"NO_DIR");
	data->aspect= '\0';
	data->fg = 0;
	data->bg = 0;
	data->row = 0;
	data->col = 0;
	data->imp = 0;
	data->power = 0;
	data->brakes = 0;
	data->mass = 0;
	data->link0id = 0;
	data->link1id = 0;
	return data;
}
 void freedata(V_data data){
	free(data);
}
