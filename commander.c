/*
  commander.c
  Interface to keyboard and sim
  -------------------------------------
*/
#include "letrain.h"

Commander c_new(){
	Commander com = malloc(sizeof(struct commander));
	com->sim = NULL;
	com->finder = f_new(NULL);
	com->mode = TRAINS_MODE;
	return com;
}
void c_free(Commander com){
	f_free(com->finder);
	free(com);
}
void c_setsim(Commander com, Sim sim){
	com->sim = sim;
}

/*
c_key: interpreta la tecla pulsada

modo DRIVERS
	hjkl       : seleccionar locomotoras en la página actual
    space      : acelerar
	.          : decelerar
	b          : incrementar frenos
	B          : decrementar frenos 
	space      : reverse motor

modo POINTS: 
	hjkl       : seleccionar cruces en la página actual
	.          : cambiar a la siguiente dirección posible del cruce
	
modo TRAINS:
	j          : avanzar remolcando
	k          : avanzar vacio
	space      : cambio de sentido
	0          : enganchar o desenganchar enganche anterior
	1          : enganchar o desenganchar enganche posterior
	a          : agregar un nuevo vehículo sin motor
	A          : agregar un nuevo vehículo con motor
	x          : eliminar el vehículo
	,          : cargar 
	.          : descargar 

todos
	HJKL       : seleccionar página del mapa
	d          : modo DRIVERS
	p          : modo POINTS
	t          : modo TRAINS
	Esc        : salir (save y/n) salva la posición de todos los vehículos
*/

void c_key(Commander com, char key){
	int col=0;
	int row =0;
	Vehicle newvehicle = NULL;
	Vehicle actual = NULL;
	Iterator crosses;
	// ALL MODES:
	switch(key){
	case 'H':
		col = s_mapcol(com->sim);
		if(col >=com->sim->mapcols/3){
			s_selmapcol(com->sim, col - com->sim->mapcols/3);
		}
		break;
	case 'J':
		row = s_maprow(com->sim);
		if(row <= com->sim->maprows){
			s_selmaprow(com->sim,row+ com->sim->maprows/3);
		}
		break;
	case 'K':
		row = s_maprow(com->sim);
		if(row >= com->sim->maprows/3){
			s_selmaprow(com->sim,row - com->sim->maprows/3);
		}
		break;
	case 'L':
		col = s_mapcol(com->sim);
		if(col <= com->sim->mapcols){
			s_selmapcol(com->sim, col + com->sim->mapcols/3);
		}
		break;
	case 'd':
		com->mode = DRIVERS_MODE;
		break;
	case 'p':
		com->mode = POINTS_MODE;
		break;
	case 't':
		com->mode = TRAINS_MODE;
		break;
	case 'q':
		s_stop(com->sim);
		break;
	default:
		switch(com->mode){
		case DRIVERS_MODE:
			switch(key){
			case 'j':
				s_prevloco(com->sim);	
				break;
			case 'k':
				s_nextloco(com->sim);	
				break;
			case 'h':
				s_incpower(com->sim);
				break;
			case 'l':
				s_decpower(com->sim);
				break;
			case 'b':
				s_incbrakes(com->sim);
				break;
			case 'B':
				s_decbrakes(com->sim);
				break;
			case ' ':
				s_reverse(com->sim);
				break;
			}
			break;
		case POINTS_MODE: 
			switch(key){
			case 'j':
				s_prevcross(com->sim);	
				break;
			case 'k':
				s_nextcross(com->sim);	
				break;
			case '.':
				s_chgselcross(com->sim); 
				break;
			}
			break;
		case TRAINS_MODE:
		//	jk         : ir via adelante y via atras
		//	hl  	   : mover el vehículo manualmente adelante y atras ?? (protegible?)
		//	0          : enganchar o desenganchar enganche anterior
		//	1          : enganchar o desenganchar enganche posterior
		//	a          : agregar un nuevo vehículo sin motor
		//	A          : agregar un nuevo vehículo con motor
		//	x          : eliminar el vehículo
		//	space      : cargar 
		//	.          : descargar 
			if(com->finder->rail == NULL){
				f_gotorail(com->finder, com->sim->cross->rail);
			}


			switch(key){
			case '.':
				crosses = i_new(com->sim->railmap->crosses);
				while(i_hasnext(crosses)){
					Cross c = (Cross)i_next(crosses);
					if(c->rail == com->finder->rail){
						x_nextdir(c);
					}
				}
				break;
			case 'j':
				f_forward(com->finder);
				actual = com->finder->rail->vehicle;
				if(actual != NULL){
					com->sim->vehicle = actual;
				}
				break;
			case ' ':
				f_reverse(com->finder);
				actual = com->finder->rail->vehicle;
				if(actual != NULL){
					com->sim->vehicle = actual;
					v_reverse(actual);
				}
				break;
			case 'k':
				actual = com->finder->rail->vehicle;
				if(actual != NULL){
					com->sim->vehicle = actual;
					v_setdir(actual, com->finder->dir);
					v_distpushed(actual, actual->mass, actual->mass, com->finder->dir);
					v_movepushed(actual, com->finder->dir);
					v_distlinked(actual, actual->mass, actual->mass, com->finder->dir);
					v_movelinked(actual, r_invertdir(com->finder->rail, com->finder->dir));
				}
				f_forward(com->finder);
				break;
			case '0':
				actual = com->finder->rail->vehicle;
				if(actual != NULL){
					com->sim->vehicle = actual;
					f_forward(com->finder);
					Vehicle tolink=com->finder->rail->vehicle;
					if(tolink != NULL){
						v_unlink(actual, tolink);
					}
					f_reverse(com->finder);
					f_forward(com->finder);
					f_reverse(com->finder);
				}
				break;
			case '1':
				actual = com->finder->rail->vehicle;
				if(actual != NULL){
					com->sim->vehicle = actual;
					f_forward(com->finder);
					Vehicle tolink=com->finder->rail->vehicle;
					if(tolink != NULL){
						v_link(actual, tolink);
					}
					f_reverse(com->finder);
					f_forward(com->finder);
					f_reverse(com->finder);
				}
				break;
			case 'a':
				if(com->finder->rail->vehicle == NULL){
					newvehicle = s_addwagon(com->sim);
					com->sim->vehicle = newvehicle;
					v_setdir(newvehicle, NO_DIR);
					v_gotorail(newvehicle, com->finder->rail);
				}
				break;
			case 'A':
				if(com->finder->rail->vehicle == NULL){
					newvehicle= s_addloco(com->sim);
					com->sim->vehicle = newvehicle;
					v_setdir(newvehicle, NO_DIR);
					v_gotorail(newvehicle, com->finder->rail);
				}
				break;
			case 'x':
				if(com->finder->rail->vehicle != NULL){
					actual=com->finder->rail->vehicle;
					com->sim->vehicle = actual;
					if(actual != NULL){
						s_del(com->sim);	
						com->finder->rail->vehicle = NULL;
						com->sim->vehicle = NULL;
						actual = NULL;
						// provisionalmente tomamos la siguiente loco
						s_nextloco(com->sim);	
					}
				}
				break;
			}
			break;
		}
	}
}
