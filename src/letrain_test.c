/*
  letrain_test.c
  general test
  -------------------------------------
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "test.h"
#include "letrain.h"
Sim sim;


/*
      01234567890123456789
	 0\ /  \|    |/  \|/
	 1 +    +    +    + 
	 2 |    |    |    | 
	 3                  
	 4 |    |    |    | 
	 5 +    +    +    + 
	 6/ \  /|    |\  /|\
	 7                  
	 8          \      /
	 9 .-  -.    .-  -. 
	10/      \          
	11                  
	12  /    /         /
	13-+   -+-  -+-  -+-                             
	14  \         \    \                             
	15                                               
	16\    \         \                               
	17-+   -+-  -+-  -+-
	18/         /    /  
	19                  
	20-:    :-                  
	21                        
	22 |    :                  
	23 :    |                  
	24      
    25  .--------.    
    26 /          \  
    27.            . 
    28|            | 
    29|            | 
    30.            . 
    31 \          /  
    32  .--------. 
*/    
void test_charmap() {
	/*
      01234567890123456789
	 0\ /  \|    |/  \|/
	 1 +    +    +    + 
	 2 |    |    |    | 
	 */
	Vehicle w = v_new(WAGON);
	Rail cross = m_rail(sim->railmap,1,1); 
	Rail out_NW = m_rail(sim->railmap,0,0); 
	Rail in_S = m_rail(sim->railmap,2,1); 
	v_gotorail(w, in_S);
	v_setdir(w, N);
	v_forward(w);
	fail_unless(w->rail == cross);
	v_forward(w);
	fail_unless(w->rail == out_NW);
	v_free(w);
}

void test_rail() {
	Rail r1= r_new(STOP_RAIL,          1, 1);
	Rail r2= r_new(VERTICAL_RAIL,      2, 2);
	Rail r3= r_new(HORIZONTAL_RAIL,    3, 3);
	Rail r4= r_new(UP_DIAGONAL_RAIL,   5, 5);
	Rail r5= r_new(DOWN_DIAGONAL_RAIL, 4, 4);
	Rail r6= r_new(CURVE_RAIL,         6, 6);
	Rail r7= r_new(CROSS_RAIL,         7, 7);

	r_setdir(r1, N, S);
	r_setdir(r1, S, S);
	fail_unless(r_dir(r1, N) == S);
	fail_unless(r_dir(r1, S) == S);

	r_setdir(r2, N, S);
	fail_unless(r_dir(r2, N) == S);
	r_setdir(r2, S, N);
	fail_unless(r_dir(r2, S) == N);

	r_setdir(r3, E, W);
	fail_unless(r_dir(r3, E) == W);
	r_setdir(r3, W, E);
	fail_unless(r_dir(r3, W) == E);

	r_setdir(r4, NE, SW);
	fail_unless(r_dir(r4, NE) == SW);
	r_setdir(r4, SW, NE);
	fail_unless(r_dir(r4, SW) == NE);
	
	r_setdir(r5, NW, SE);
	fail_unless(r_dir(r5, NW) == SE);
	r_setdir(r5, SE, NW);
	fail_unless(r_dir(r5, SE) == NW);
	
	r_setdir(r6, N, SE);
	fail_unless(r_dir(r6, N) == SE);
	r_setdir(r6, SE, N);
	fail_unless(r_dir(r6, SE) == N);

	r_setdir(r7, N, SE);
	fail_unless(r_dir(r7, N) == SE);
	r_setdir(r7, SE, N);
	r_setdir(r7, S, N);
	r_setdir(r7, SW, N);
	fail_unless(r_dir(r7, SE) == N);
	fail_unless(r_dir(r7, S) == N);
	fail_unless(r_dir(r7, SW) == N);
	r_free(r1);
	r_free(r2);
	r_free(r3);
	r_free(r4);
	r_free(r5);
	r_free(r6);
	r_free(r7);
}


void test_list() {
	int a= 11;
	int b= 22;
	int c= 33;
	void * data[] = {&a, &b, &c};
	List l = l_new();
	l_add(l, data[0]);
	fail_unless(l_contains(l, data[0]));

	l_add(l, data[1]);
	fail_unless(l_contains(l, data[0]));
	fail_unless(l_contains(l, data[1]));

	l_add(l, data[2]);
	fail_unless(l_contains(l, data[0]));
	fail_unless(l_contains(l, data[1]));
	fail_unless(l_contains(l, data[2]));

	Listdata d= NULL;
	Iterator i = i_new(l);
	while(i_hasnext(i)){
		d= i_next(i);
		fail_unless(d!= NULL);
	}
	i_free(i);

	fail_unless(l_get(l, 2) == data[0]); 
	fail_unless(l_get(l, 1) == data[1]); 
	fail_unless(l_get(l, 0) == data[2]); 

	l_del(l, data[2]);
	fail_unless(!l_contains(l, data[2]));

	l_del(l, data[1]);
	fail_unless(!l_contains(l, data[1]));

	l_del(l, data[0]);
	fail_unless(!l_contains(l, data[0]));

	l_free(l);
}


void test_railmap() {

	fail_unless(m_rail(sim->railmap, 1,0) == NULL);	
	fail_unless(m_rail(sim->railmap, 0,0)->type == DOWN_DIAGONAL_RAIL);	
	fail_unless(m_rail(sim->railmap, 0,2)->type == UP_DIAGONAL_RAIL);	
	fail_unless(m_rail(sim->railmap, 1,1)->type == CROSS_RAIL);	
	fail_unless(m_rail(sim->railmap, 2,1)->type == VERTICAL_RAIL);	
	fail_unless(m_rail(sim->railmap,5 ,1)->type == CROSS_RAIL);	
	fail_unless(m_rail(sim->railmap,9 ,1)->type == CURVE_RAIL);	
	fail_unless(m_rail(sim->railmap,13,0)->type == HORIZONTAL_RAIL);	
	fail_unless(m_rail(sim->railmap,13,1)->type == CROSS_RAIL);	
	fail_unless(m_rail(sim->railmap,17,1)->type == CROSS_RAIL);	
	fail_unless(m_rail(sim->railmap,20,1)->type == STOP_RAIL);	

	Rail cross = m_rail(sim->railmap,1,1); 
	Rail out_NW = m_rail(sim->railmap,0,0); 
	Rail out_NE= m_rail(sim->railmap,0,2); 
	Rail in_S = m_rail(sim->railmap,2,1); 
	fail_unless(r_link(cross, NW) == out_NW);
	fail_unless(r_link(cross, NE) == out_NE);
	fail_unless(r_link(cross,  S) == in_S);

	cross = m_rail(sim->railmap,1,6); 
	out_NW = m_rail(sim->railmap,0,5); 
	Rail out_N = m_rail(sim->railmap,0,6); 
	in_S = m_rail(sim->railmap,2,6); 
	fail_unless(r_link(cross, NW) == out_NW);
	fail_unless(r_link(cross, N ) == out_N );
	fail_unless(r_link(cross,  S) == in_S);

	cross = m_rail(sim->railmap,1,11); 
	out_N  = m_rail(sim->railmap,0,11); 
	out_NE = m_rail(sim->railmap,0,12); 
	in_S = m_rail(sim->railmap,2,11); 
	fail_unless(r_link(cross, N) == out_N);
	fail_unless(r_link(cross, NE ) == out_NE );
	fail_unless(r_link(cross,  S) == in_S);
}


void test_railVehicle() {
	/*
    26 /          \  
    27.            . 
    28|            | 
	*/
	Vehicle w = v_new(WAGON);
	Rail out_NE = m_rail(sim->railmap,26,1); 
	Rail cross = m_rail(sim->railmap,27,0); 
	Rail in_S = m_rail(sim->railmap,28,0); 
	fail_unless(cross->vehicle == NULL);
	fail_unless(out_NE->vehicle == NULL);
	fail_unless(in_S->vehicle == NULL);

	v_setdir(w, N);
	v_gotorail(w, in_S);
	fail_unless(cross->vehicle == NULL);
	fail_unless(out_NE->vehicle == NULL);
	fail_unless(in_S->vehicle == w);

	v_forward(w);
	fail_unless(cross->vehicle == w);
	fail_unless(out_NE->vehicle == NULL);
	fail_unless(in_S->vehicle == NULL);
	fail_unless(w->rail == cross);

	v_forward(w);
	fail_unless(cross->vehicle == NULL);
	fail_unless(out_NE->vehicle == w);
	fail_unless(in_S->vehicle == NULL);
	fail_unless(w->rail == out_NE);
	v_free(w);
}


void test_v_gotorail() {

	Rail r = NULL;
	Rail apart = r_new(STOP_RAIL, 0,0);	
	Vehicle w = v_new(WAGON);

	r = r_new(STOP_RAIL, 0,0);	
	v_setdir(w, N);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == NO_DIR);
	v_gotorail(w,apart);
	v_setdir(w, S);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == NO_DIR);
	v_gotorail(w,apart);
	v_setdir(w, E);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == NO_DIR);
	v_gotorail(w,apart);
	v_setdir(w, W);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == NO_DIR);
	v_gotorail(w,apart);
	r_free(r);

	r = r_new(VERTICAL_RAIL, 0,0);
	v_setdir(w, S);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == S);
	v_gotorail(w,apart);
	v_setdir(w, N);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == N);
	v_gotorail(w,apart);
	v_setdir(w, E);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == NO_DIR);
	v_gotorail(w,apart);
	v_setdir(w, W);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == NO_DIR);
	v_gotorail(w,apart);
	r_free(r);

	r = r_new(HORIZONTAL_RAIL, 0,0);
	v_setdir(w, E);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == E);
	v_gotorail(w,apart);
	v_setdir(w, W);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == W);
	v_gotorail(w,apart);
	v_setdir(w, N);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == NO_DIR);
	v_gotorail(w,apart);
	v_setdir(w, S);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == NO_DIR);
	v_gotorail(w,apart);
	r_free(r);

	r = r_new(DOWN_DIAGONAL_RAIL, 0,0);
	v_setdir(w, SE);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == SE);
	v_gotorail(w,apart);
	v_setdir(w, NW);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == NW);
	v_gotorail(w,apart);
	v_setdir(w, N);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == NO_DIR);
	v_gotorail(w,apart);
	v_setdir(w, S);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == NO_DIR);
	v_gotorail(w,apart);
	v_setdir(w, E);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == NO_DIR);
	v_gotorail(w,apart);
	v_setdir(w, W);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == NO_DIR);
	v_gotorail(w,apart);
	r_free(r);

	r = r_new(UP_DIAGONAL_RAIL, 0,0);
	v_setdir(w, NE);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == NE);
	v_gotorail(w,apart);
	v_setdir(w, SW);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == SW);
	v_gotorail(w,apart);
	v_setdir(w, N);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == NO_DIR);
	v_gotorail(w,apart);
	v_setdir(w, S);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == NO_DIR);
	v_gotorail(w,apart);
	v_setdir(w, E);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == NO_DIR);
	v_gotorail(w,apart);
	v_setdir(w, W);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == NO_DIR);
	v_gotorail(w,apart);
	r_free(r);

	r = r_new(CURVE_RAIL, 10,10);
	r_setdir(r, N, SW);
	r_setdir(r, SW, N);
	v_setdir(w, S);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == SW);
	v_gotorail(w,apart);
	v_setdir(w, NE);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == N);
	v_gotorail(w,apart);
	r_free(r);

	r = r_new(CROSS_RAIL, 10,10);
	r_setdir(r, N, SW);
	r_setdir(r, SW, N);
	r_setdir(r, S, N);
	r_setdir(r, SE, N);
	v_setdir(w, S);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == SW);
	v_gotorail(w,apart);
	v_setdir(w, NE);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == N);
	v_gotorail(w,apart);
	r_free(r);
	v_free(w);

}


void test_crossPoint() {
	Rail apart = r_new(CROSS_RAIL, 0,0);
	Rail r = r_new(CROSS_RAIL, 10,10);
	Vehicle w = v_new(WAGON);
	int outs[] = {SE, S, SW};
	Cross c = x_new(r, N, outs);
	v_setdir(w, S);
	x_setdir(c, 0);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == SE);
	v_gotorail(w,apart);
	v_setdir(w, S);
	x_setdir(c, 1);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == S);
	v_gotorail(w,apart);
	v_setdir(w, S);
	x_setdir(c, 2);
	v_gotorail(w,r);
	fail_unless(v_dir(w) == SW);
	v_gotorail(w,apart);
	x_free(c);
	v_free(w);
	r_free(r);
	r_free(apart);
}


void test_commander() {
	Commander c = sim->commander;

	fail_unless(s_maprow(sim) == 0);
	fail_unless(s_mapcol(sim) == 0);

	c_key(c, 'H');
	fail_unless(s_maprow(sim) == 0);
	fail_unless(s_mapcol(sim) == 0);

	c_key(c, 'K');
	fail_unless(s_maprow(sim) == 0);
	fail_unless(s_mapcol(sim) == 0);

	c_key(c, 'J');
	fail_unless(s_maprow(sim) == 1);
	fail_unless(s_mapcol(sim) == 0);

	c_key(c, 'L');
	fail_unless(s_maprow(sim) == 1);
	fail_unless(s_mapcol(sim) == 1);

	c_key(c, 'H');
	fail_unless(s_maprow(sim) == 1);
	fail_unless(s_mapcol(sim) == 0);

	c_key(c, 'K');
	fail_unless(s_maprow(sim) == 0);
	fail_unless(s_mapcol(sim) == 0);

	c_key(c, 'd');
	fail_unless(sim->commander->mode == DRIVERS_MODE);
	c_key(c, 'p');
	fail_unless(sim->commander->mode == POINTS_MODE);
	c_key(c, 't');
	fail_unless(sim->commander->mode == TRAINS_MODE);


	c_key(c, 'd');
	//DRIVERS
	fail_unless(sim->commander->mode == DRIVERS_MODE);
	Vehicle selected = s_vehicle(sim);
	fail_unless(selected != NULL);
	c_key(c, 'k');
	Vehicle other = s_vehicle(sim);
	fail_unless(other != NULL);
	fail_unless(selected != other );

	c_key(c, 'p');
	//POINTS
	fail_unless(sim->commander->mode == POINTS_MODE);
	Cross selectedcross = s_cross(sim);
	c_key(c, 'k'); Cross nextcross = s_nextcross(sim);
	fail_unless(sim->commander->mode == POINTS_MODE);
	fail_unless(selectedcross != nextcross ); 
}

void test_mass() {
	Vehicle v0 = s_selvehicle(sim, 0);
	Vehicle v1 = s_selvehicle(sim, 1);
	Vehicle v2 = s_selvehicle(sim, 2);
	Vehicle v3 = s_selvehicle(sim, 3);
	v0->dir = E;
	v1->dir = E;
	v2->dir = E;
	v3->dir = E;

	fail_unless(v_linkedmass(v0, E) == 10);
	v_link(v0, v1);
	fail_unless(v_linkedmass(v0, E) == 20);
	v_link(v1, v2);
	fail_unless(v_linkedmass(v0, E) == 30);
	v_link(v2, v3);
	fail_unless(v_linkedmass(v0, E) == 40);
	v_distlinked(v0, 400, 40, 0);
	fail_unless(v_imp(v0) == 100); 
	fail_unless(v_imp(v1) == 100); 
	fail_unless(v_imp(v2) == 100); 
	fail_unless(v_imp(v3) == 100); 
}

void test_link() {
	Vehicle v0 = s_selvehicle(sim, 0);
	Vehicle v1 = s_selvehicle(sim, 1);
	Vehicle v2 = s_selvehicle(sim, 2);
	Vehicle v3 = s_selvehicle(sim, 3);
	v0->dir = E;
	v1->dir = N;
	v2->dir = E;
	v3->dir = NW;
	s_selvehicle(sim, 0);
	v_link(v0, v1);
	fail_unless(v_linked(v0, v1));
	fail_unless(!v_linked(v1, v2));
	fail_unless(!v_linked(v2, v3));

}

void test_distlinked() {
	Vehicle v0 = s_selvehicle(sim, 0);
	Vehicle v1 = s_selvehicle(sim, 1);
	Vehicle v2 = s_selvehicle(sim, 2);
	Vehicle v3 = s_selvehicle(sim, 3);
	v0->imp = 0;
	v1->imp = 0;
	v2->imp = 0;
	v3->imp = 0;
	v_link(v0, v1);
	v_link(v1, v2);
	v_link(v2, v3);
	v_distlinked(v0,400, 40,0); 
	fail_unless(v0->imp == 100); 
	fail_unless(v1->imp == 100); 
	fail_unless(v2->imp == 100); 
	fail_unless(v3->imp == 100); 

	v_unlink(v0, v1);
	v_unlink(v1,v2);
	v_unlink(v2,v3);
	v0->imp = 0;
	v1->imp = 0;
	v2->imp = 0;
	v3->imp = 0;
	v_link(v0, v1);
	v_distlinked(v0,400, 20,0); 
	fail_unless(v0->imp == 200); 
	fail_unless(v1->imp == 200); 
	fail_unless(v2->imp == 0); 
	fail_unless(v3->imp == 0); 
}


void test_distpushed() {
	Vehicle v0 = s_selvehicle(sim, 0);
	Vehicle v1 = s_selvehicle(sim, 1);
	Vehicle v2 = s_selvehicle(sim, 2);
	Vehicle v3 = s_selvehicle(sim, 3);
	v0->dir = E;
	v1->dir = N;
	v2->dir = W;
	v3->dir = E;
	v_distpushed(v0,1000, 40,0); 
	fail_unless(v0->dir == E); 
	fail_unless(v1->dir == E); 
	fail_unless(v2->dir == E); 
	fail_unless(v3->dir == E); 
	v_forward(v3);
  	v3->dir = N;
  	v_distpushed(v0,1000, 40,0); 
  	fail_unless(v0->dir == E); 
  	fail_unless(v1->dir == E); 
  	fail_unless(v2->dir == E); 
  	fail_unless(v3->dir == N); 
}


void test_movlinked() {
	Vehicle v0 = s_selvehicle(sim, 0);
	Vehicle v1 = s_selvehicle(sim, 1);
	Vehicle v2 = s_selvehicle(sim, 2);
	Vehicle v3 = s_selvehicle(sim, 3);
	v0->dir = E;
	v1->dir = E;
	v2->dir = E;
	v3->dir = E;
	v_link(v0, v1);
	v_link(v1, v2);
	v_link(v2, v3);
	v_distlinked(v0,1000, 40,0); 
	fail_unless(v_movlinked(v3, W) == 1);
	fail_unless(v0->pos.col == 5);
	fail_unless(v1->pos.col == 6);
	fail_unless(v2->pos.col == 7);
	fail_unless(v3->pos.col == 8);
}

void test_movpushed() {
	Vehicle v0 = s_selvehicle(sim, 0);
	Vehicle v1 = s_selvehicle(sim, 1);
	Vehicle v2 = s_selvehicle(sim, 2);
	Vehicle v3 = s_selvehicle(sim, 3);
	v0->dir = E;
	v1->dir = E;
	v2->dir = E;
	v3->dir = E;
	v_distpushed(v0,1000, 40,E); 
	fail_unless(v_movpushed(v0, E) == 1);
	fail_unless(v0->pos.col == 4); // el primero no se empuja
	fail_unless(v1->pos.col == 6);
	fail_unless(v2->pos.col == 7);
	fail_unless(v3->pos.col == 8);
}

void setup (void) {
	sim = s_new();
	s_loadmap(sim, "test_groundmap.txt");
	Rail ra=NULL;
	struct point p = {25,4};
	s_addwagon(sim);
	s_addwagon(sim);
	s_addloco(sim);
	s_addloco(sim);

	Vehicle next = s_selvehicle(sim, 0);
	v_setdir(next, E);
	ra = m_rail(sim->railmap, p.row, p.col); 
	fail_unless(ra != NULL);
	v_gotorail(next, ra);

	p.col++;
	next = s_selvehicle(sim, 1);
	fail_unless(next != NULL);
	v_setdir(next, N);
	ra = m_rail(sim->railmap, p.row, p.col); 
	fail_unless (ra != NULL);
	v_gotorail(next, ra);

	p.col++;
	next = s_selvehicle(sim, 2);
	fail_unless(next != NULL);
	v_setdir(next, N);
	ra = m_rail(sim->railmap, p.row, p.col); 
	fail_unless(ra != NULL);
	v_gotorail(next, ra);

	p.col++;
	next = s_selvehicle(sim, 3);
	fail_unless(next != NULL);
	v_setdir(next, N);
	ra = m_rail(sim->railmap, p.row, p.col); 
	fail_unless(ra != NULL);
	v_gotorail(next, ra);

	s_nextcross(sim);

	s_selmapcol(sim, 0);
	s_selmaprow(sim, 0);
}

void cleanup (void) {
	s_free(sim);
}
void (*tests[])() = {
	test_charmap,
 	test_rail,
	test_list,
	test_railmap,
	test_railVehicle,
	test_v_gotorail,
	test_crossPoint,
	test_commander,
	test_mass,
	test_link,
	test_distlinked,
	test_distpushed,
	test_movlinked,
	test_movpushed,
	NULL 
};
int main(int argc, char* argv[]){
	int numfails = 0;
	startlog("test.log");
	numfails = dotestset(tests);
	if(numfails > 0){
		printf("Total fails: %d\n", numfails);
	}
	stoplog();
	return numfails;
}
