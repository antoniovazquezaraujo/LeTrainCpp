#include <stdio.h>
#include <string.h>
#include "letrain.h"

typedef struct v_data * V_data;
struct v_data{
	int id;
	int type;
	char dir[7];
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
static void v_dataread(V_data data, FILE *file, int type);
static void v_datawrite(V_data data, FILE *file);
static Vehicle v_datatovehicle(V_data data);
static void v_vehicletodata(Vehicle v, V_data data);
static char * dirtostr(int dir);
static int strtodir(char *c);
static V_data newdata();
static void freedata(V_data data);

int loadmap(Sim s, FILE * mapfile){ 
	Charmap charmap = h_new();
	int numRow= 0; 
	int numCol= 0; 
	int maxcols = 0;
	char *line = malloc(CHAR_MAP_COLS); 
	assert( mapfile!= NULL);
	while(fgets(line, CHAR_MAP_COLS, mapfile) != NULL){
		maxcols = strlen(line);
		if(numCol > maxcols) {
			maxcols = numCol;
		}
		for(numCol = 0; line[numCol] != '\n'; numCol++){
			h_setchar(charmap,numRow,numCol, line[numCol]);
		}
		numRow++;
	}
	free(line);
	s->railmap = rm_new(charmap);
	free(charmap);
	s->i_crosses = i_new(s->railmap->crosses);
	s->maprows = numRow;
	s->mapcols=  maxcols;
	return numRow;
}
void loadgame(Sim s, FILE * gamefile){	
	V_data data = newdata();
	Vehicle v = NULL;
	int n=0;
	char section[40];
	if(gamefile != NULL){
		while(1){
			n = fscanf(gamefile, "%s",section);
			if(n == EOF){
				break;
			}
			if(!strcmp(section, "locomotive")){
				v_dataread(data, gamefile, LOCOMOTIVE);
				v = v_datatovehicle(data);
				s_addvehicle(s, v);
			}else if(!strcmp(section, "wagon")){
				v_dataread(data, gamefile, WAGON);
				v = v_datatovehicle(data);
				s_addvehicle(s, v);
			}else{
				printf("error\n");
			}
		}
	}
	freedata(data);
}
void savegame(Sim s, FILE * gamefile){
	V_data data = newdata();
	Vehicle v = NULL;
	Iterator i = i_new(s->vehicles);
	while(i_hasnext(i)){
		v = i_next(i);
		v_vehicletodata(v, data);
		v_datawrite(data, gamefile);
	}
	i_free(i);
	freedata(data);
}

static void v_dataread(V_data data, FILE *file, int type){
	char dummy[20];
	int id;
	char dir[7]; 
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
static void v_datawrite(V_data data, FILE *file){
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

static Vehicle v_datatovehicle(V_data data){
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
static void v_vehicletodata(Vehicle v, V_data data){
	data->id = v->id;
	data->type = v->type;
	strcpy(data->dir, dirtostr(v->dir));
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
}

static char * dirtostr(int dir){
	char *dirnames[] = {"E","NE","N","NW","W","SW","S","SE","NO_DIR"};
	char * ret = malloc(7);
	if(dir != NO_DIR){
		strcpy(ret, dirnames[dir]);
	}else{
		strcpy(ret, dirnames[8]);
	}
	return ret;
	/*
	char * strdir= NULL;
	char * ret = malloc(7);
	switch(dir){
	case E:
		strdir = "E";
		break;
	case NE:
		strdir = "NE";
		break;
	case N:
		strdir = "N";
		break;
	case NW:
		strdir = "NW";
		break;
	case W:
		strdir = "W";
		break;
	case SW:
		strdir = "SW";
		break;
	case S:
		strdir = "S";
		break;
	case SE:
		strdir = "SE";
		break;
	default:
		strdir = "NO_DIR";
	}
	strcpy(ret, strdir);
	return ret;
	*/
}
static int strtodir(char *c){
	int ret = 0;
	if(     !strcmp(c,"E" )) ret = E;
	else if(!strcmp(c,"NE")) ret = NE;
	else if(!strcmp(c,"N" )) ret = N;
	else if(!strcmp(c,"NW")) ret = NW;
	else if(!strcmp(c,"W" )) ret = W;
	else if(!strcmp(c,"SW")) ret = SW;
	else if(!strcmp(c,"S" )) ret = S;
	else if(!strcmp(c,"SE")) ret = SE;
	else ret = NO_DIR;
	return ret;
}
static V_data newdata(){
	V_data data = malloc(sizeof(struct v_data));
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
static void freedata(V_data data){
	free(data);
}
