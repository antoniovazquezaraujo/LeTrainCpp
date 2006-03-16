/*
  sim.c
  The real simulator
  -------------------------------------
*/


#include "letrain.h"
#include "graphics.h"
static int running = 0;
Sim s_new(){
	Sim s = malloc(sizeof(struct sim));
	s->railmap = m_new();
	s->vehicle=NULL;
	s->cross = NULL;
	s->rail = NULL;
	s->locomotives = l_new();
	s->i_locomotives = i_new(s->locomotives);
	s->vehicles = l_new();
	s->i_vehicles = i_new(s->vehicles);
	s->i_crosses = NULL;
	s->maprow = 0;
	s->mapcol=0;
	s->commander = c_new();
	c_setsim(s->commander, s);
	return s;
}
void s_free(Sim s){
	assert(s->i_locomotives != NULL);
	i_free(s->i_locomotives);
	Iterator i = i_new(s->vehicles);
	while(i_hasnext(i)){
		Vehicle v = (Vehicle) i_next(i);
		v_free(v);
	}
	i_free(i);
	l_free(s->vehicles);
	l_free(s->locomotives);
	assert(s->i_vehicles!= NULL);
	i_free(s->i_vehicles);
	assert(s->i_crosses!= NULL);
	i_free(s->i_crosses);
	c_free(s->commander);
	m_free(s->railmap);
	free(s);
}
int s_loadmap(Sim s, char * filename){
	int n = m_load(s->railmap, filename);
	s->i_crosses = i_new(s->railmap->crosses);
	return n;
}
int s_loadgame(Sim s, char * filename){
	/*

	Rail ra=NULL;
	struct point p = {6,2};
	assert(p.row == 6 && p.col == 2);
	s_addwagon(s);
	s_addwagon(s);
	s_addloco(s);
	s_addloco(s);
	Vehicle next = s_selvehicle(s, 0);
	assert(next != NULL);
	v_setdir(next, N);
	ra = m_rail(s->railmap, p.row, p.col); 
	assert(ra != NULL);
	v_gotorail(next, ra);

	p.row++;
	assert(p.row == 7 && p.col == 2);
	next = s_selvehicle(s, 1);
	assert(next != NULL);
	v_setdir(next, N);
	ra = m_rail(s->railmap, p.row, p.col); 
	assert (ra != NULL);
	v_gotorail(next, ra);

	p.row++;
	assert(p.row == 8 && p.col == 2);
	next = s_selvehicle(s, 2);
	assert(next != NULL);
	v_setdir(next, N);
	ra = m_rail(s->railmap, p.row, p.col); 
	assert(ra != NULL);
	v_gotorail(next, ra);

	p.row++;
	assert(p.row == 9 && p.col == 2);
	next = s_selvehicle(s, 3);
	assert(next != NULL);
	v_setdir(next, N);
	ra = m_rail(s->railmap, p.row, p.col); 
	assert(ra != NULL);
	v_gotorail(next, ra);
	*/

	s_nextcross(s);

	s_selmapcol(s, 0);
	s_selmaprow(s, 0);
	return 0;
}
int s_save(Sim s){
	return 0;
}
void s_start(Sim s){
	running = 1;
	s_cycle(s);
}
void s_stop(Sim s){
	running = 0;
}
Vehicle s_selvehicle(Sim s, int vehicleIndex){
	s->vehicle = l_get(s->vehicles, vehicleIndex);
	return s->vehicle;
}
Vehicle s_vehicle(Sim s){
	return s->vehicle;
}
void s_incpower(Sim s){
	s->vehicle->power++;
}
void s_decpower(Sim s){
	if(s->vehicle->power >0){
		s->vehicle->power--;
	}
}
int s_incbrakes(Sim s){
	int brakes = v_brakes(s->vehicle);
	brakes++;
	v_setbrakes(s->vehicle, brakes);
	return brakes;
}
int s_decbrakes(Sim s){
	int brakes = v_brakes(s->vehicle);
	if(brakes > 0) brakes--;
	v_setbrakes(s->vehicle, brakes);
	return brakes;
}
int s_reverse(Sim s){
	v_reverse(s->vehicle);
	return 0;
}
void s_del(Sim s){
	l_del(s->vehicles, s->vehicle);
	//Seleccionar el siguiente aqui
	return ;
} 
Vehicle s_addloco(Sim s){
	Vehicle v = v_new(LOCOMOTIVE);
	l_add(s->locomotives, v);
	l_add(s->vehicles, v);
	return v;
}
Vehicle s_addwagon(Sim s){
	Vehicle v = v_new(WAGON);
	l_add(s->vehicles, v);
	return v;
}
Vehicle s_nextloco(Sim s){
	if(!i_hasnext(s->i_locomotives)){
		i_reset(s->i_locomotives, s->locomotives);
	}
	if(i_hasnext(s->i_locomotives)){
		s->vehicle = (Vehicle) i_next(s->i_locomotives);
	}
	return s->vehicle;
}
Vehicle s_prevloco(Sim s){
	// We need a double linked list!
	return NULL;
}
Rail s_rail(Sim s){
	return NULL;
}
void s_selrail(Sim s, Rail r){
	return;
}
void s_selmaprow(Sim s, int row){
	s->maprow = row;
}
int s_maprow(Sim s){
	return s->maprow;
}
void s_selmapcol(Sim s, int col){
	s->mapcol = col;
}
int s_mapcol(Sim s){
	return s->mapcol;
}
void s_selcross(Sim s, Cross cp){
	return;
}
Cross s_nextcross(Sim s){
	if(!i_hasnext(s->i_crosses)){
		i_reset(s->i_crosses, s->railmap->crosses);
	}
	if(i_hasnext(s->i_crosses)){
		s->cross= (Cross) i_next(s->i_crosses);
	}
	return s->cross;
}
Cross s_prevcross(Sim s){
	//We need a double linked list!
	return NULL;
}
Cross s_cross(Sim s){
	return s->cross;
}
void s_chgselcross(Sim s){
	x_nextdir(s->cross);
} 
void s_movetrains(Sim sim){
	Vehicle next = NULL;
	int antidir= 0;
	int totalmass= 0;
	int totalimp = 0;
	int consum = 0;
	Iterator i = i_new(sim->locomotives);
	while(i_hasnext(i)){
		consum = 0;
		next = (Vehicle)i_next(i);
		antidir=r_dir(next->rail, next->dir);
		totalimp = next->power;
		totalmass = v_pushedmass(next, next->dir) + v_linkedmass(next, antidir);
		v_distlinked(next, totalimp, totalmass, antidir);
		v_distpushed(next, totalimp, totalmass, next->dir);
		if(v_pushedcanmove(next, next->dir) && v_linkedcanmove(next, antidir)){
			v_movepushed(next, next->dir);
			v_movelinked(next, antidir);
		}	
	}
	i_free(i);

}
void s_cycle(Sim s){
	while(running){
		s_doactions(s);
		s_movetrains(s);
		s_draw(s);
	}
}
void s_doactions(Sim s){
	int key = g_getkey();
	if(key!= -1){
		c_key(s->commander, key);
	}
}
void s_draw(Sim s){
	s_clearrails(s);
	s_drawrails(s);
	s_drawcrosses(s);
	s_drawvehicles(s);
	s_drawfinder(s);
	s_drawstatus(s);
	s_drawhelp(s);
	g_repaint();
}
void s_drawhelp(Sim sim){
	mvprintw(42,0,"[d:DRIVERS] [p:CROSSPOINTS] [t:TRAINS] [q:QUIT]" ); 
	mvprintw(43,0,"                                                                                                              " ); 
	switch(sim->commander->mode){
	case DRIVERS_MODE:	
		mvprintw(43,0,"j(prev loco) k(next loco) space(rev) h(inc imp) l(dec imp) b(inc brk) B(dec brk)" ); 
		break;
	case TRAINS_MODE:	
		mvprintw(43,0,"j(go) space(rev) k(move) 1(link) 0(unlink) .(crosspoint) a(add wagon) A(add loco) x(del)"); 
		break;
	case POINTS_MODE:	
		mvprintw(43,0,"j(prev cross) k(next_cross) .(change direction)"); 
		break;
	}

}
void s_drawstatus(Sim sim){
	char * modes[] = {
		"DRIVERS", 
		"CROSSPOINTS",
		"TRAINS"
	};
	char * mode = modes[sim->commander->mode];
	Vehicle v = sim->vehicle;
	if(v == NULL){
		return;
	}
	char aspect = v->aspect;
	int dir = v->dir;

	char links[6];
	if(v->links[0]){
		links[0] = v->links[0]->aspect;
		links[1] = '<';
	}else{
		links[0] = ' ';
		links[1] = ' ';
	}
	links[2] = v->aspect;
	if(v->links[1]){
		links[3] = '>';
		links[4] = v->links[1]->aspect;
	}else{
		links[3] = ' ';
		links[4] = ' ';
	}
	links[5] = '\0';
	
	int imp = v->imp;
	int power= v->power;
	int brk = v->brakes;
	char railaspect = v->rail->aspect;
	int row = v->pos.row;
	int col = v->pos.col;

	mvprintw(39,0,"Actual mode:%s                           ", mode ); 
	mvprintw(40,0,"                                                                                       "); 
	mvprintw(40,0,"V:%c dir:%d, links: %s, pow: %d imp:%d, brk:%d rail:%c, row:%d, col:%d", aspect,dir,links,power,imp,brk,railaspect,row,col); 
}
void s_drawfinder(Sim s){
	if(s->commander->finder == NULL){
		return;
	}
	if(s->commander->finder->rail == NULL){
		return;
	}
	Rail r = s->commander->finder->rail;
	if(r->vehicle != NULL){
		g_putc(
			(r->vehicle->pos.row)+START_ROW, 
			(r->vehicle->pos.col)+START_COL, 
			r->vehicle->aspect,
			DEF_FINDER_FG,
			DEF_FINDER_BG	
		);
	}else{
		g_putc(
			(r->pos.row)+START_ROW, 
			(r->pos.col)+START_COL, 
			r->aspect,
			DEF_FINDER_FG,
			DEF_FINDER_BG	
		);
	}
}
void s_drawvehicles(Sim s){
	Iterator i = i_new(s->vehicles);
	while(i_hasnext(i)){
		Vehicle next = (Vehicle)i_next(i);
		if(next == s_vehicle(s)){
			g_putc(
				(next->pos.row)+START_ROW, 
				(next->pos.col)+START_COL, 
				next->aspect,
				next->bg,
				next->fg
			);
		}else{
			g_putc(
				(next->pos.row)+START_ROW, 
				(next->pos.col)+START_COL, 
				next->aspect,
				next->fg,
				next->bg
			);
		}
	}
	assert(i != NULL);
	i_free(i);
}
void s_drawrails(Sim s){
	Rail r = NULL;
	int n=0;
	int m=0;
	for(n = 0; n<RAIL_MAP_ROWS; n++){
		for(m = 0; m<RAIL_MAP_COLS; m++){
			r = s->railmap->rails[n][m];
			if(r != NULL){
				g_putc(
					(r->pos.row)+START_ROW, 
					(r->pos.col)+START_COL, 
					r->aspect,
					DEF_RAIL_FG,
					DEF_RAIL_BG 
				);
			}
		}
	}
}
void s_drawcrosses(Sim s){
	Rail r = NULL;
	Cross next = NULL;
	Iterator i = i_new(s->railmap->crosses);
	while(i_hasnext(i)){
		next = (Cross)i_next(i);
		r = next->rail;
		if(next == s->cross){
			g_putc(
				(r->pos.row)+START_ROW, 
				(r->pos.col)+START_COL, 
				r->aspect,
				DEF_CROSS_BG,
				DEF_CROSS_FG 
			);
		}else{
			g_putc(
				(r->pos.row)+START_ROW, 
				(r->pos.col)+START_COL, 
				r->aspect,
				DEF_CROSS_FG,
				DEF_CROSS_BG 
			);
		}
	}
	i_free(i);
}
		
void s_clearrails(Sim s){
	Rail r = NULL;
	int n=0;
	int m=0;
	for(n = 0; n<RAIL_MAP_ROWS; n++){
		for(m = 0; m<RAIL_MAP_COLS; m++){
			r = s->railmap->rails[n][m];
			if(r != NULL){
				g_putc(
					(r->pos.row)+START_ROW, 
					(r->pos.col)+START_COL, 
					' ',
					DEF_RAIL_BG,
					DEF_RAIL_FG
				);
			}
		}
	}
}
