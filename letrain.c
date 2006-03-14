/*===========================================================================*/
/*
 *  			  ----------------------------------------
 *  			  .    .--- --.-- .---.     .     .  
 *  			  |    |      |   |   |    / \    | .\   |
 *  			  |    |---   |   .---.   /   \   | | \  |
 *  			  |    |      |   |  \   .-----.  | |  \ |
 *  			  .--- .---   .   |   \ /       \ | |   \.
 *  			  ----------------------------------------
 *                     The Letter Train Simulator	
 *                Copyright (C) 2006 Antonio Vazquez Araujo 
                    antoniovazquezaraujo[at]gmail[dot]com
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
#include "letrain.h"
#include "graphics.h"
#include "logging.h"
int main (void) {
	startlog("letrain.log");
	g_init();
	Sim sim = s_new();
	s_loadmap(sim,"groundmap.txt");
	s_loadgame(sim,"game1.txt");
	s_start(sim);
	g_reset();
	s_free(sim);
	stoplog();
	return 0;
}
