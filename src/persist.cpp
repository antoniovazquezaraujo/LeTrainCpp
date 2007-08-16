/*
  persist.cpp
  The persistence manager 
  -------------------------------------
*/
#include "persist.h"
using namespace std;
int loadmap(Sim * s, char * filename){ 
	ifstream mapfile(filename);
	Charmap * cm = new Charmap();
	int numRow= 0; 
	int numCol= 0; 
	int maxcols = 0;
	char *line = new char[CHAR_MAP_COLS]; 
	while((mapfile.getline(line, CHAR_MAP_COLS)) && (!mapfile.eof() )){
	//while(fgets(line, CHAR_MAP_COLS, mapfile) != 0){
		maxcols = strlen(line);
		if(numCol > maxcols) {
			maxcols = numCol;
		}
		for(numCol = 0; numCol < maxcols; numCol++){
			cm->h_setchar(numRow,numCol, line[numCol]);
		}
		numRow++;
	}
	delete [] line;

	s->railmap = new OldRailmap(cm);
	delete cm;
        
	s->i_crosses = s->railmap->crosses->iterator();
	s->maprows = numRow;
	s->mapcols=  maxcols;
	return numRow;
}
void loadgame(Sim * s, char * filename){	
	ifstream gamefile(filename);
	if(!gamefile){
		cout << "Ehhhhhhhh. No hay fichero!" << endl;
		cin.get();
		return ;
	}
	V_data data = newdata();
	Vehicle*  v = 0;
	char section[40];
	while(gamefile.getline(section, 40) && !gamefile.eof()){
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
	freedata(data);
}
void savegame(Sim * s, char * filename){
	ofstream gamefile(filename);
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

void dataread(V_data data, ifstream &file, int type){
	char propertyName[100];
	data->type = type; 
	file >> 
	   propertyName >> data->id 
	>> propertyName >> data->dir 
	>> propertyName >> data->aspect 
	>> propertyName >> data->fg 
	>> propertyName >> data->bg 
	>> propertyName >> data->row 
	>> propertyName >> data->col 
	>> propertyName >> data->imp
	>> propertyName >> data->power 
	>> propertyName >> data->brakes 
	>> propertyName >> data->mass 
	>> propertyName >> data->link0id 
	>> propertyName >> data->link1id; 
}
void datawrite(V_data data, ofstream &file){
	char type[20];
	strcpy(type, data->type==LOCOMOTIVE?"locomotive":"wagon");
	file << 
	   type
	<< data->id
	<< data->dir
	<< data->aspect
	<< data->fg
	<< data->bg
	<< data->row
	<< data->col
	<< data->imp
	<< data->power
	<< data->brakes
	<< data->mass
	<< data->link0id
	<< data->link1id;
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
