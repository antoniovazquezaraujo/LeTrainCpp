/*
  letrain.h 
  The general declarations 
  -------------------------------------
*/
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "list.h"
#include "logging.h"

#define CHAR_MAP_ROWS 100 
#define CHAR_MAP_COLS 100 
#define MAX_PAGE_ROWS 40
#define MAX_PAGE_COLS 40
#define MAX_ROWS 80
#define MAX_COLS 80
#define START_ROW 10
#define START_COL 10
#define RAIL_MAP_ROWS 100 
#define RAIL_MAP_COLS 100 

#define DEF_WAGON_BG 0 
#define DEF_WAGON_FG 3 
#define DEF_LOCOMOTIVE_BG 0 
#define DEF_LOCOMOTIVE_FG 4 
#define DEF_RAIL_BG 0 
#define DEF_RAIL_FG 4  
#define DEF_CROSS_FG 6 
#define DEF_CROSS_BG 0 
#define DEF_FINDER_FG 0 
#define DEF_FINDER_BG 7 

typedef struct vehicle    *Vehicle;
typedef struct finder     *Finder;
typedef struct commander  *Commander;
typedef struct sim        *Sim;
typedef struct triterator *Triterator;
typedef struct point      *Point;
typedef struct charmap    *Charmap;
typedef struct rail       *Rail;
typedef struct cross      *Cross;
typedef struct railmap    *Railmap;

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
	WAGON
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
 Point
*/
struct point{
	int row;
	int col;
};
void       p_move(Point p, int dir);
/*
 Rail
 Rail, representa un rail de la via
*/
struct rail{
	int type;
	char aspect;
	struct point pos; 
	int dirs[8];
	Rail links[8];     
	Vehicle vehicle;
};
Rail       r_new(int type, int row, int col);
void       r_free(Rail r);
void       r_setlink(Rail r, int dir, Rail link);
Rail       r_link(Rail r, int dir);
void       r_setdir(Rail r, int dir, int value);
int        r_dir(Rail r, int dir);
int        r_invertdir(Rail rail, int dir);
int        r_validdir(Rail rail);
/*
 Cross
 Representa un cruce de vías. Tiene una entrada
 y hasta tres salidas, pero se puede cruzar 
 en sentido contrario.
*/
struct cross{
	Rail rail;
	int in;
	int outs[3];
	int out;
};
Cross      x_new(Rail r, int keydir, int outs[]);
void       x_free(Cross cp);
void       x_setdir(Cross cp, int index);
void       x_nextdir(Cross cp);
void       x_updaterail(Cross cross);
int        x_dir(Cross cp);
int        x_keydir(Cross cp);
Rail       x_rail(Cross cp);


// Direction
int        d_invert(int dir);

/*
 Railmap
 Plano x, y de railes
*/
struct railmap{
	Rail rails[RAIL_MAP_ROWS][RAIL_MAP_COLS];
	List crosses;
};
Railmap    m_new();
void       m_free(Railmap rm);
Rail       m_rail(Railmap rm, int row, int col);
void       m_setrail(Railmap rm, int row, int col, Rail r);
void       m_addcross(Railmap rm, Cross cp);
List       m_crosslist(Railmap rm);
int        m_load(Railmap rm, char * filename);
/*
 Finder 
 Cursor to select rails and vehicles 
*/

struct finder{
	int dir;
	Rail rail;
	char aspect;
	int fg;
	int bg;
	struct point pos;
};
Finder     f_new(Vehicle v);
void       f_free(Finder f);
void       f_gotorail(Finder f, Rail r);
int        f_forward(Finder f);
void       f_reverse(Finder f);
/*
 Vehicle
 Objeto que viaja a una velocidad con un impulso, frenos, etc.
*/
struct vehicle{
	int type;
	int dir;
	Rail rail;
	char aspect;
	int fg;
	int bg;
	struct point pos;
	int imp;
	int power;
	int brakes;
	int reversed;
	int mass;
	Vehicle links[2];
};
Vehicle    v_new(int type);
void       v_free(Vehicle v);
Point      v_pos(Vehicle v);
void       v_setpos(Vehicle v, Point pos);
void       v_consum(Vehicle v);
int        v_imp(Vehicle v);
void       v_setimp(Vehicle v, int imp);
int        v_brakes(Vehicle v);
void       v_setbrakes(Vehicle v, int brakes);
int        v_mass(Vehicle v);
void       v_link(Vehicle a, Vehicle b);
void       v_unlink(Vehicle a, Vehicle b);
int        v_linked(Vehicle a, Vehicle b);
int        v_gotorail(Vehicle v, Rail r);
Vehicle    v_atdir(Vehicle v, int dir);
int        v_linkedcanmove(Vehicle v, int dir);
int        v_pushedcanmove(Vehicle v, int dir);
int        v_canmove(Vehicle v);
//void       v_forwardtrain(Vehicle v);
int        v_forward(Vehicle v);
void       v_reverse(Vehicle v);
void       v_setdir(Vehicle v, int dir);
int        v_dir(Vehicle v);
int        v_linkedmass(Vehicle v, int dir);
int        v_pushedmass(Vehicle v, int dir);
int        v_distlinked(Vehicle v, int imp, int totalmass, int dir);
int        v_movelinked(Vehicle v, int dir);
int        v_distpushed(Vehicle v, int imp, int totalmass, int dir);
int        v_movepushed(Vehicle v, int dir);
//Triterator. Train iterator
Triterator t_new(Vehicle v, int dir);
void       t_free(Triterator tri);
int        t_hasnext(Triterator tri);
Vehicle    t_next(Triterator tri);
void       t_reset(Triterator tri);

/*
 Commander
 Interfaz de comandos para el simulador
*/
struct commander{
	Sim sim;
	Finder finder;
	int mode;
};
Commander  c_new();
void       c_free();
void       c_setsim(Commander c, Sim s);
void       c_key(Commander c, char key);

/*
 Sim
 Simulador de trenes, el propio juego.
*/
struct sim{
	Railmap railmap;
	Commander commander;
	Vehicle vehicle;
	Cross cross;
	int maprow;
	int mapcol;
	Rail rail;
	List locomotives;
	Iterator i_locomotives;
	List vehicles;
	Iterator i_vehicles;
	Iterator i_crosses;
};
Sim        s_new();
void       s_free(Sim s);
int        s_loadmap(Sim s, char * filaname);
int        s_loadgame(Sim s, char * filaname);
int        s_save(Sim s);
void       s_start(Sim s);
void       s_stop(Sim s);
Vehicle    s_selvehicle(Sim s, int vehicleindex);
Vehicle    s_vehicle(Sim s); //selected vehicle

// operations with the selected vehicle
void       s_incpower(Sim s);
void       s_decpower(Sim s);
int        s_incbrakes(Sim s);
int        s_decbrakes(Sim s);
int        s_invimp(Sim s);
int        s_link(Sim s);
int        s_unlink(Sim s);
void       s_del(Sim s); 

// adding vehicles
Vehicle    s_addloco(Sim s);
Vehicle    s_addwagon(Sim s);

//selecting vehicles
Vehicle    s_nextloco(Sim s);
Vehicle    s_prevloco(Sim s);

//selecting rails
Rail       s_rail(Sim s);
void       s_selrail(Sim s, Rail r);

//operations with the map
void       s_selmaprow(Sim s, int row);
int        s_maprow(Sim s);
void       s_selmapcol(Sim s, int col);
int        s_mapcol(Sim s);

//operations with crosses
void       s_selcross(Sim s, Cross cp);
Cross      s_nextcross(Sim s);
Cross      s_prevcross(Sim s);
Cross      s_cross(Sim s);
void       s_chgselcross(Sim s); 

//
void       s_movetrains(Sim s);
void       s_cycle(Sim s);
void       s_doactions(Sim s);
void       s_draw(Sim s);
void       s_drawhelp(Sim sim);
void       s_drawstatus(Sim s);
void       s_drawfinder(Sim s);
void       s_drawvehicles(Sim s);
void       s_drawrails(Sim s);
void       s_drawcrosses(Sim s);
void       s_clearrails(Sim s);
