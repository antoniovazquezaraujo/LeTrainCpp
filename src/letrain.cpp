/*===========================================================================*/
/*
 *                      ________________________________
 *                          __ ____  _     _           
 *                    /   /_    /   / /  /__/  /  /\ /
 *                   /__ /__   /   / \  /  /  /  /  /
 *                  ________________________________
 *                     The Letter Train Simulator	
 *                Copyright (C) 2006 Antonio Vazquez Araujo 
 *                  antoniovazquezaraujo[at]gmail[dot]com
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the license or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the Gnu Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 */
/*===========================================================================*/
#include <iostream> 
#include "letrain.h"
#include "persist.h"
#include "cpptest.h"
#include "sim.h"
#include "RailTestSuite.h"
void test(){
	RailTestSuite r;
}
using namespace std;
int main() {
	test();
	startlog("letrain.log");
	g_init();
	Sim * sim = new Sim();
	loadmap(sim, "groundmap.txt");
	loadgame(sim,"game1.txt");
	sim->setup();
	sim->start();
	savegame(sim, "game1.txt");
	delete sim;
	g_reset();
	stoplog();
	return 0;
}
