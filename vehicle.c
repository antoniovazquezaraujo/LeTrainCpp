/*
  vehicle.c
  A wagon or locomotive. 
  -------------------------------------
*/
#include "letrain.h"
static int serialnumber=0;
Vehicle v_new(int type){
	Vehicle v = malloc(sizeof(struct vehicle));
	v->id = v_newid();
	v->rail = NULL;
	v->type= type;
	v->pos.row=0;
	v->pos.col=0;
	switch(type){
	case LOCOMOTIVE:	
		v->aspect = 'L';
		v->fg = DEF_LOCOMOTIVE_FG;
		v->bg = DEF_LOCOMOTIVE_BG;
		break;
	case WAGON:	
		v->aspect =  'a'+ (char)(26.0 * rand()/(RAND_MAX)); 
		v->fg =  (int)(8.0 * rand()/(RAND_MAX)); 
		v->bg = DEF_WAGON_BG;
		break;
	}

	v->dir = NO_DIR;
	v->imp = 0;
	v->power = 0;
	v->brakes = 0;
	v->mass= 10; 
	v->links[0] = NULL;	
	v->links[1] = NULL;	
	return v;
}
int v_newid(){
	extern int serialnumber;
	return ++serialnumber;
}
void v_free(Vehicle v){
	free(v);
}
Point v_pos(Vehicle v){
	return &(v->pos);
}
void v_setpos(Vehicle v, Point pos){
	v->pos.row = pos->row;
	v->pos.col= pos->col;
}
void v_setdir(Vehicle v, int dir){
	v->dir = dir;
}
void v_consum(Vehicle v){
	if(v->imp >= v->mass){
		v->imp-= v->mass;
	}
}
int v_dir(Vehicle v){
	return v->dir;
}
int v_imp(Vehicle v){
	return v->imp;
}
void v_setimp(Vehicle v, int imp){
	v->imp = imp;
}
int v_brakes(Vehicle v){
	return v->brakes;
}
void v_setbrakes(Vehicle v, int brakes){
	v->brakes = brakes;
}
int v_mass(Vehicle v){
	return v->mass;
}

void v_link(Vehicle a, Vehicle b){
	v_unlink(a,b);
	if(a->links[0] == NULL){
		a->links[0] = b;
	}else if(a->links[1] == NULL){
		a->links[1] = b;
	}else{
		assert(0);
	}
	if(b->links[0] == NULL){
		b->links[0] = a;
	}else if(b->links[1] == NULL){
		b->links[1] = a;
	}else{
		assert(0);
	}
}
void v_unlink(Vehicle a, Vehicle b){
	if(a->links[0] == b){
		a->links[0] = NULL;
	}else if(a->links[1] == b){
		a->links[1] = NULL;
	}
	if(b->links[0] == a){
		b->links[0] = NULL;
	}else if(b->links[1] == a){
		b->links[1] = NULL;
	}

}
int v_linked(Vehicle a, Vehicle b){
	assert(a != NULL);
	assert(b != NULL);
	int atob= 0;
	int btoa= 0;
	if(a->links[0] == b){
		atob=1;	
	}else if(a->links[1] == b){
		atob=1;
	}
	if(b->links[0] == a){
		btoa=1;	
	}else if(b->links[1] == a){
		btoa=1;
	}
	return (atob && btoa);

}
int v_gotorail(Vehicle v, Rail r){
	int inputDir = NO_DIR;
	if(r->vehicle != NULL){
		return 0;
	}
	if(v->rail != NULL){
		v->rail->vehicle = NULL;
	}
	if(v->dir != NO_DIR){
		inputDir = d_invert(v->dir);
	}else{
		inputDir = r_validdir(r);
	}
	v->dir = r_dir(r, inputDir);
	assert (v->dir != NO_DIR);
	v->pos.row = r->pos.row;
	v->pos.col = r->pos.col;
	v->rail = r;
	r->vehicle = v;
	return 1;
}
Vehicle v_atdir(Vehicle v, int dir){
	Finder f = f_new(v);
	f->dir = v->dir;
	f_forward(f);
	Vehicle found  = f->rail->vehicle;
	f_free(f);
	return found; 
}
int v_pushedcanmove(Vehicle v, int dir){
	int ret=1;
	Finder finder = f_new(v);
	Vehicle next = NULL;
	finder->dir = dir;
	f_forward(finder);
	next = finder->rail->vehicle;
	while(next != NULL){
		next->dir = finder->dir;
		if(f_forward(finder)){
			next = finder->rail->vehicle;
		}else{
			next = NULL;
		}
	}
	f_reverse(finder);
	f_forward(finder);
	next = finder->rail->vehicle;
	while(next != NULL && next != v){ //move all yast to BEFORE v !
		if(! v_canmove(next)){
			ret = 0;
			break;
		}
		f_forward(finder);
		next = finder->rail->vehicle;
	}
	f_free(finder);
	return ret;

}
int v_canmove(Vehicle v){
	if(v->imp >= v->mass){
		Rail r = r_link(v->rail, v->dir);
		if( r == NULL){
			lg("v_canmove",1,"no se mueve %c en dir: %d, en %d, %d", v->aspect, v->dir,v->pos.row, v->pos.col);
			return 0;
		}
	}else{
		return 0;
	}
	return 1;
}
int v_linkedcanmove(Vehicle v, int dir){
	int ret = 1;
	Vehicle next = NULL;
	Triterator t = t_new(v, dir);
	while(t_hasnext(t)){
		next = t_next(t);
		if(! v_canmove(next)){
			ret = 0;
			break;
		}else{
		}
	}
	t_free(t);
	return ret;
}
int v_forward(Vehicle v){
	int ret = 0;
	if (v == NULL){
		return ret;
	}
	Rail next = r_link(v->rail, v->dir);
	if( next != NULL){
		if (v_gotorail(v, next)){
			ret = 1;
		}else{
			ret = 0;
		}
	}else{
		ret = 0;
	}
	return ret;
}
void v_reverse(Vehicle v){
	v->dir = r_dir(v->rail, v->dir);
}
int v_linkedmass(Vehicle v, int dir){
	Vehicle next = NULL;
	int totalmass = 0;
	Triterator t = t_new(v, dir);
	while(t_hasnext(t)){
		next = t_next(t);
		totalmass += v_mass(next);
	}
	t_free(t);
	return totalmass;
}
int v_pushedmass(Vehicle v, int dir){
	Vehicle next = NULL;
	int totalmass = 0;
	Finder finder = f_new(v);
	f_forward(finder);
	next = finder->rail->vehicle;
	while(next != NULL){
		totalmass += v_mass(next);
		f_forward(finder);
		next = finder->rail->vehicle;
	}
	f_free(finder);
	return totalmass;
}
int v_distlinked(Vehicle v, int imp, int totalmass, int dir){
	int totcons=0;
	float cons =0;
	Vehicle next = NULL;
	Finder finder = f_new(v);
	Triterator t = t_new(v, dir);
	while(t_hasnext(t)){
		next = t_next(t);
		cons= (imp * v_mass(next) ) / totalmass;
		next->imp += (int)cons;
		f_forward(finder);
		totcons+=cons;
	}
	t_free(t);
	f_free(finder);
	return totcons;
}
int v_movelinked(Vehicle v, int dir){
	v_orientlinked(v, dir);
	int ret = 1;
	Vehicle next = NULL;
	Triterator t = t_new(v, dir);
	while(t_hasnext(t)){
		next = t_next(t);
		if(! v_forward(next)){
			ret = 0;
			break;
		}else{
			v_consum(next);
		}
	}
	t_free(t);
	return ret;
}
void v_orientlinked(Vehicle v, int dir){
	Vehicle next = NULL;
	Finder finder = f_new(v);
	f_reverse(finder);
	Triterator t = t_new(v, dir);
	while(t_hasnext(t)){
		next = t_next(t);
		f_reverse(finder);
		v_setdir(next, finder->dir);
		f_reverse(finder);
		f_forward(finder);
	}
	t_free(t);
	f_free(finder);
}
int v_distpushed(Vehicle v, int imp, int totalmass, int dir){
	int totcons=0;
	int cons =0;
	Finder finder = f_new(v);
	Vehicle next = NULL;
	f_forward(finder);
	next = finder->rail->vehicle;
	while(next != NULL){
		cons= (imp * v_mass(next) ) / totalmass;
		next->imp += cons;
		totcons+=cons;
		v_setdir(next, finder->dir);
		f_forward(finder);
		next = finder->rail->vehicle;
	}
	f_free(finder);
	return totcons;
}

int v_movepushed(Vehicle v, int dir){
	int ret=1;
	Finder finder = f_new(v);
	Vehicle next = NULL;
	finder->dir = dir;
	f_forward(finder);
	next = finder->rail->vehicle;
	while(next != NULL){
		next->dir = finder->dir;
		if(f_forward(finder)){
			assert(finder->dir != NO_DIR);
			next = finder->rail->vehicle;
		}else{
			next = NULL;
		}
	}
	assert(finder->dir != NO_DIR);
	f_reverse(finder);
	assert(finder->dir != NO_DIR);
	f_forward(finder);
	next = finder->rail->vehicle;
	while(next != v){ //move all yast to BEFORE v !
		if( ! v_forward(next)){
			ret = 0;
			break;
		}else{
			v_consum(v);
		}
		f_forward(finder);
		next = finder->rail->vehicle;
	}
	f_free(finder);
	return ret;
}

//Triterator. Train iterator
struct triterator{
	Vehicle start;
	Finder finder;
	Vehicle actual;
	Vehicle next;
	Vehicle prev;
	int dir;
};
Triterator t_new(Vehicle v, int dir){
	Triterator t = malloc(sizeof(struct triterator));
	t->finder = f_new(v);
	t->dir = dir;
	t->start = v;
	t_reset(t);
	return t;
}
void t_free(Triterator t){
	f_free(t->finder);
	free(t);
}
int t_hasnext(Triterator t){
	return t->actual != NULL;
}
Vehicle t_next(Triterator t){
	Vehicle oldactual = t->actual;
	t->prev = t->actual;
	if(t->next != NULL && v_linked(t->actual, t->next)){
		if(t->next->links[0] != NULL && t->next->links[0] != oldactual){
			t->actual = t->next;
			t->next = t->next->links[0];
		}else if(t->next->links[1] != NULL && t->next->links[1] != oldactual){
			t->actual = t->next;
			t->next = t->next->links[1];
		}else{
			t->actual = t->next; 
			t->next = NULL; 
		}
	}else{
		t->actual = NULL; 
	}
	return t->prev;
}
void t_reset(Triterator t){
	t->actual = t->start;
	t->prev = t->start;
	f_gotorail(t->finder, t->actual->rail); 
	t->finder->dir = t->dir;
	if(f_forward(t->finder)){
		t->next = t->finder->rail->vehicle;
	}else{
		t->next = NULL;
	}
}
