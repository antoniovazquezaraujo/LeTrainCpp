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
#include <csignal>
#include "Basic.h"
#include "Game.h"
#include "Window.h"
//Window * Game::statusWindow;
void fin(int n);
int main(){
	signal(SIGINT, fin);
	Window::setup();
	Game g;
	g.start();
	fin(0);
}
void fin(int n){
	Window::cleanup();
	exit(0);
}
