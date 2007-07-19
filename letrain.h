/*
  letrain.h 
  The general declarations 
  -------------------------------------
*/
#ifndef letrain_h
#define letrain_h

//#include <stdlib.h>
//#include <stdio.h>
//#include <math.h>

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

#define DEF_WAGON_BG 0 
#define DEF_WAGON_FG 3 
#define DEF_LOCOMOTIVE_BG 0 
#define DEF_LOCOMOTIVE_FG 7 
#define DEF_RAIL_BG 0 
#define DEF_RAIL_FG 4  
#define DEF_CROSS_FG 6 
#define DEF_CROSS_BG 0 
#define DEF_FINDER_FG 0 
#define DEF_FINDER_BG 5 

typedef struct data    *V_data;
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

enum{
	NO_DIR =-1,
	E   =0,
	NE  =1,
	N   =2,
	NW  =3,
	W   =4,
	SW  =5,
	S   =6,
	SE  =7
};
// Vehicle types
enum{
	LOCOMOTIVE, 
	WAGON,
	CROSS
};
// Commander modes
enum{
	DRIVERS_MODE, 
	POINTS_MODE,
	TRAINS_MODE
};
// Rail types
enum{
	STOP_RAIL, 
	VERTICAL_RAIL,
	HORIZONTAL_RAIL,
	DOWN_DIAGONAL_RAIL,
	UP_DIAGONAL_RAIL,
	CURVE_RAIL,
	CROSS_RAIL
};
 /*
 	Node 
 */
class Node {
public:
	Node();
	~Node();
	Node * prev;
	void * data;
	Node * next;
};
/*
 	List 
*/
class List{
	friend class Iterator;
public:
	List(); 
	~List(); 
	void       add(void * data);
	void       insert(void * data);
	void       append(void * data);
	void       del(void * data);
	int        contains(void * data);
	void *     get(int index);
	Iterator * iterator();
private:
	Node * start;
	Node * end;
};
class Iterator{
public:
	Iterator(List * list);
	~Iterator();
	int    hasNext();
	int    hasPrev();
	void * next();
	void * prev();
	void   reset();
	void   toEnd();
private:
	Node * actualNode;
	List * list;
};

/*
 Point
*/
class Point{
public:
	Point(): row(0), col(0){

	}
	Point(int row, int col): row(row), col(col){
	}
	int row;
	int col;
	void  move(int dir);
};

/*
 Rail
 Rail, representa un rail de la via
*/
class Rail{
public:
	Rail (int type, int row, int col);
	~Rail();
	void setlink(int dir, Rail * link);
	Rail * link(int dir);
	void setDir(int dir, int value);
	int invertDir(int dir);
	int getDir(int dir);
	int validDir();

	int type;
	char aspect;
	Point pos; 
	int dirs[8];
	Rail * links[8];     
	Vehicle*  vehicle;
};
/*
 Cross
 Representa un cruce de vías. Tiene una entrada
 y hasta tres salidas, pero se puede cruzar 
 en sentido contrario.
*/
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

//TrainIterator. Train iterator
class TrainIterator{
public:
	TrainIterator(Vehicle*  v, int dir);
	~TrainIterator();
	void reset();
	int hasNext();
	Vehicle*  next();
private:
	Vehicle*  start;
	Finder*   f;
	Vehicle*  actual;
	Vehicle*  nextVehicle;
	Vehicle*  prev;
	int dir;
};


// Direction
int invert(int dir);

/*
 Charmap
 Plano en ASCII 
*/
class Charmap{
public:
	Charmap();
	~Charmap();
	char h_char(int row, int col); 
	void h_setchar(int row, int col, char c); 
	char data[CHAR_MAP_ROWS][CHAR_MAP_COLS];
};

/*
 Railmap
 Plano x, y de railes
*/
class Railmap{
public:
	Railmap (Charmap * charmap);
	~Railmap ();
	int load(Charmap * charmap);
	void addCross(Cross * cp);
	List * crosslist();
	void setRail(int row, int col, Rail * r);
	Rail * getRail(int row, int col);
	Rail * rails[RAIL_MAP_ROWS][RAIL_MAP_COLS];
	List * crosses;
private:
	int lookaround (Charmap * charmap, int row, int col, int directions[][3]);
	Rail * newcurve(Charmap * charmap, int row, int col);
	struct railandcross newCross(Charmap * charmap, int row, int col);
	Rail * newstop(Charmap * charmap, int row, int col);
	int crossouts (int directions[][3], int outs[3], int keyDir);
	void connect();
	void setcrosses(List * crosses);
	int getCrossDir(int directions[][3]);
};
/*
 Finder 
 Cursor to select rails and vehicles 
*/

class Finder{
public:
	Finder (Vehicle*  v);
	~Finder();
	void gotoRail(Rail * r);
	int forward();
	void reverse();

	int dir;
	Rail * rail;
	char aspect;
	int fg;
	int bg;
	Point pos;
};
/*
 Vehicle* 
 Objeto que viaja a una velocidad con un impulso, frenos, etc.
*/
class Vehicle{
public:
	Vehicle    (int type);
	~Vehicle   ();
	int        newid();
	Point *    getPos();
	void       setPos(Point * pos);
	float      getImp();
	void       setImp(float imp);
	void       incImp(float imp=1);
	int        getBrakes();
	void       setBrakes(int brakes);
	int        getMass();
	void       link(Vehicle*  b);
	void       unlink(Vehicle*  b);
	int        linked(Vehicle*  b);
	int        gotoRail(Rail * r);
	int        linkedcanmove();
	int        pushedcanmove();
	int        canmove();
	int        forward();
	void       reverse();
	void       setDir(int dir);
	int        getDir();
	int        linkedMass(int dir);
	int        pushedMass();
	void       braketrain();
	float      distlinked(float imp);
	int        movelinked();
	void       orientlinked(int dir);
	float      distpushed(float imp);
	int        movepushed();

	int id;
	int type;
	int dir;
	Rail * rail;
	char aspect;
	int fg;
	int bg;
	Point pos;
	float imp;
	int power;
	int brakes;
	int mass;
	Vehicle*  links[2];
	int link0id;
	int link1id;
	int moved;
	float speed;
	int turns;
	int position;
	float friction;
	int maxTurns;
};

/*
 Commander
 Interfaz de comandos para el simulador
*/
class Commander{
public:
	Commander  ();
	~Commander ();
	void c_setsim(Sim * s);
	void c_key(char key);

	Sim * sim;
	Finder* finder;
	int mode;
};

/*
 Sim
 Simulador de trenes, el propio juego.
*/
class Sim{
public:
	Sim        ();
	~Sim       ();
	void       setup();
	void       free();
	int        loadmap(char * filaname);
	int        loadgame(char * filaname);
	int        save();
	void       start();
	void       stop();
	Vehicle*   vehiclebyid(int id);
	void       linkvehicles();
	Vehicle*   selVehicle(int vehicleindex);
	Vehicle*   getVehicle(); //selected vehicle

	// operations with the selected vehicle
	void       incpower();
	void       decpower();
	int        incBrakes();
	int        decBrakes();
	int        reverse();
	int        link();
	int        unlink();
	void       del(); 

	// adding vehicles
	void       addVehicle(Vehicle*  v);
	Vehicle*   addloco();
	Vehicle*   addwagon();

	//selecting vehicles
	Vehicle*   nextloco();
	Vehicle*   prevloco();

	//selecting rails
	Rail *     getRail();

	//operations with the map
	void       selectMapRow(int row);
	int        getMapRow();
	void       selectMapCol(int col);
	int        getMapCol();

	//operations with crosses
	Cross *    nextCross();
	Cross *    prevCross();
	Cross *    getCross();
	void       changeSelectedCross(); 

	//
	void       movetrains();
	void       cycle();
	void       doactions();
	void       draw();
	void       drawhelp();
	void       updatevisor();
	void       drawstatus();
	void       drawfinder();
	void       drawVehicles();
	void       drawrails();
	void       drawcrosses();
	void       clearrails();

	Railmap * railmap;
	Commander *  commander;
	Vehicle*  vehicle;
	Cross * cross;
	int maprow;
	int mapcol;
	int maprows;
	int mapcols;
	int statusrow;
	int statuscol;
	int statusrows;
	int statuscols;
	Rail * rail;
	List * locomotives;
	Iterator * i_locomotives;
	Iterator * i_crosses;
	List * vehicles;
};
/*
 Persist 
 Persistence manager 
*/
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

int loadmap(Sim * s, FILE * mapfile); 
void loadgame(Sim * s, FILE *gamefile);	
void savegame(Sim * s, FILE *gamefile);

int strToDir(char *c);
void dirtostr(int dir, char* dest);
void dataread(V_data data, FILE *file, int type);
void datawrite(V_data data, FILE *file);
Vehicle*  dataToVehicle(V_data data);
void vehicletodata(Vehicle*  v, V_data data);
V_data newdata();
void freedata(V_data data);

#endif
