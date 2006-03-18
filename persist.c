#include <stdio.h>
#include <string.h>
#include "letrain.h"

typedef struct v_data * V_data;
struct v_data{
	int id;
	int type;
	char * dir;
	char aspect;
	int fg;
	int bg;
	int row;
	int col;
	int imp;
	int power;
	int brakes;
	int mass;
	int link0id;
	int link1id;
};

static int strtodir(char *c);
static char * dirtostr(int dir);
static V_data v_dataread(FILE *file, int type);
static void v_datawrite(V_data data, FILE *file);
static Vehicle v_datatovehicle(V_data data);
static V_data v_vehicletodata(Vehicle v);
static char * dirtostr(int dir);
static int strtodir(char *c);

int loadmap(Sim s, FILE * mapfile){ 
	Charmap charmap = h_new();
	int numRow= 0; 
	int numCol= 0; 
	char *line = malloc(CHAR_MAP_COLS); 
	assert( mapfile!= NULL);
	while(fgets(line, CHAR_MAP_COLS, mapfile) != NULL){
		for(numCol = 0; line[numCol] != '\n'; numCol++){
			h_setchar(charmap,numRow,numCol, line[numCol]);
		}
		numRow++;
	}
	free(line);
	s->railmap = m_new(charmap);
	free(charmap);
	s->i_crosses = i_new(s->railmap->crosses);
	return numRow;
}
void loadgame(Sim s, FILE * gamefile){	
	V_data data = NULL;
	Vehicle v = NULL;
	int n=0;
	char section[40];
	while(1){
		n = fscanf(gamefile, "%s",section);
		if(n == EOF){
			break;
		}
		if(!strcmp(section, "locomotive")){
			data = v_dataread(gamefile, LOCOMOTIVE);
			v = v_datatovehicle(data);
			s_addvehicle(s, v);
		}else if(!strcmp(section, "wagon")){
			data = v_dataread(gamefile, WAGON);
			v = v_datatovehicle(data);
			s_addvehicle(s, v);
		}else{
			printf("error\n");
		}
	}
	s_linkvehicles(s);
	s_nextcross(s);
	s_selmapcol(s, 0);
	s_selmaprow(s, 0);
}
void savegame(Sim s, FILE * gamefile){
	V_data data = NULL;
	Vehicle v = NULL;
	Iterator i = i_new(s->vehicles);
	while(i_hasnext(i)){
		v = i_next(i);
		data = v_vehicletodata(v);
		v_datawrite(data, gamefile);
	}
	i_free(i);
}

V_data v_dataread(FILE *file, int type){
	char dummy[20];
	int id;
	char dir[3];
	char aspect;
	int fg;
	int bg;
	int row;
	int col;
	int imp;
	int power;
	int brakes;
	int mass;
	int link0id; 
	int link1id;

	fscanf(file, "%s %d %s %s %s %c %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d", 
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

	V_data data = malloc(sizeof(struct v_data));
	data->id = id;
	data->type = type;
	data->dir = dir;
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
	return data;
}
void v_datawrite(V_data data, FILE *file){
	fprintf(file, "%s\n id %d\n dir %s\n aspect %c\n fg %d\n bg %d\n row %d\n col %d\n imp %d\n power %d\n brakes %d\n mass %d\n link0id %d\n link1id %d\n\n",
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

Vehicle v_datatovehicle(V_data data){
	Vehicle v = v_new(data->type);
	v->id = data->id;
	v->dir = strtodir(data->dir);
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
V_data v_vehicletodata(Vehicle v){
	V_data data = malloc(sizeof(struct v_data));
	data->id = v->id;
	data->type = v->type;
	data->dir = dirtostr(v->dir);
	data->aspect = v->aspect;
	data->fg = v->fg;
	data->bg = v->bg;
	data->row = v->pos.row;
	data->col = v->pos.col;
	data->imp = v->imp;
	data->power = v->imp;
	data->brakes = v->brakes;
	data->mass = v->mass;
	data->link0id = v->links[0]!= NULL?v->links[0]->id:0;
	data->link1id = v->links[1]!= NULL?v->links[1]->id:0;
	return data;
}

char * dirtostr(int dir){
	char * ret = NULL;
	switch(dir){
	case E:
		ret = "E";
		break;
	case NE:
		ret = "NE";
		break;
	case N:
		ret = "N";
		break;
	case NW:
		ret = "NW";
		break;
	case W:
		ret = "W";
		break;
	case SW:
		ret = "SW";
		break;
	case S:
		ret = "S";
		break;
	case SE:
		ret = "SE";
		break;
	default:
		ret = "NO_DIR";
	}
	return ret;
}
int strtodir(char *c){
	int ret = 0;
	if(!strcmp(c,"E")) ret = 0;
	else if(!strcmp(c,"NE")) ret = 1;
	else if(!strcmp(c,"N")) ret = 2;
	else if(!strcmp(c,"NW")) ret = 3;
	else if(!strcmp(c,"W")) ret = 4;
	else if(!strcmp(c,"SW")) ret = 5;
	else if(!strcmp(c,"S")) ret = 6;
	else if(!strcmp(c,"SE")) ret = 7;
	else ret = NO_DIR;
	return ret;
}
