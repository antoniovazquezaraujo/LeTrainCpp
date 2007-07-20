/*
  commander.cpp
  Interface to keyboard and sim
  -------------------------------------
*/
#include "letrain.h"

Commander::Commander (){
	sim = NULL;
	finder = new Finder(NULL);
	mode = TRAINS_MODE;
}
Commander::~Commander(){
	delete finder;
}
void Commander::c_setsim(Sim * sim){
	this->sim = sim;
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

void Commander::c_key(char key){
	int col=0;
	int row =0;
	Vehicle*  newvehicle = NULL;
	Vehicle*  actual = NULL;
	Iterator * crosses;
	// ALL MODES:
	switch(key){
	case 'H':
		col = sim->getMapCol();
		if(col >=sim->mapcols/3){
			sim->selectMapCol( col - sim->mapcols/3);
		}
		break;
	case 'J':
		row = sim->getMapRow();
		if(row <= sim->maprows){
			sim->selectMapRow(row+ sim->maprows/3);
		}
		break;
	case 'K':
		row = sim->getMapRow();
		if(row >= sim->maprows/3){
			sim->selectMapRow(row - sim->maprows/3);
		}
		break;
	case 'L':
		col = sim->getMapCol();
		if(col <= sim->mapcols){
			sim->selectMapCol( col + sim->mapcols/3);
		}
		break;
	case 'd':
		mode = DRIVERS_MODE;
		break;
	case 'p':
		mode = POINTS_MODE;
		break;
	case 't':
		mode = TRAINS_MODE;
		break;
	case 'q':
		sim->stop();
		break;
	default:
		switch(mode){
		case DRIVERS_MODE:
			switch(key){
			case 'j':
				sim->prevloco();	
				break;
			case 'k':
				sim->nextloco();	
				break;
			case 'h':
				sim->incpower();
				break;
			case 'l':
				sim->decpower();
				break;
			case 'b':
				sim->incBrakes();
				break;
			case 'B':
				sim->decBrakes();
				break;
			case ' ':
				sim->reverse();
				break;
			}
			break;
		case POINTS_MODE: 
			switch(key){
			case 'j':
				sim->prevCross();	
				break;
			case 'k':
				sim->nextCross();	
				break;
			case '.':
				sim->changeSelectedCross(); 
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
			if(finder->rail == NULL){
				finder->gotoRail(sim->cross->rail);
			}


			switch(key){
			case '.':
				crosses = sim->railmap->crosses->iterator();
				while(crosses->hasNext()){
					Cross * c = (Cross *)crosses->next();
					if(c->rail == finder->rail){
						c->nextDir();
					}
				}
				break;
			case 'j':
				finder->forward();
				actual = finder->rail->vehicle;
				if(actual != NULL){
					sim->vehicle = actual;
				}
				break;
			case ' ':
				finder->reverse();
				actual = finder->rail->vehicle;
				if(actual != NULL){
					sim->vehicle = actual;
					actual->reverse();
				}
				break;
			case 'k':
				actual = finder->rail->vehicle;
				if(actual != NULL){
					sim->vehicle = actual;
					actual->setDir(finder->dir);
					actual->distpushed(actual->getMass() + actual->getBrakes());
					actual->movepushed();
					actual->distlinked(actual->getMass() + actual->getBrakes());
					actual->movelinked();
				}
				finder->forward();
				break;
			case '0':
				actual = finder->rail->vehicle;
				if(actual != NULL){
					sim->vehicle = actual;
					finder->forward();
					Vehicle*  tolink=finder->rail->vehicle;
					if(tolink != NULL){
						actual->unlink(tolink);
					}
					finder->reverse();
					finder->forward();
					finder->reverse();
				}
				break;
			case '1':
				actual = finder->rail->vehicle;
				if(actual != NULL){
					sim->vehicle = actual;
					finder->forward();
					Vehicle*  tolink=finder->rail->vehicle;
					if(tolink != NULL){
						actual->link(tolink);
					}
					finder->reverse();
					finder->forward();
					finder->reverse();
				}
				break;
			case 'a':
				if(finder->rail->vehicle == NULL){
					newvehicle = sim->addwagon();
					sim->vehicle = newvehicle;
					newvehicle->setDir(NO_DIR);
					newvehicle->gotoRail(finder->rail);
				}
				break;
			case 'A':
				if(finder->rail->vehicle == NULL){
					newvehicle= sim->addloco();
					sim->vehicle = newvehicle;
					newvehicle->setDir(NO_DIR);
					newvehicle->gotoRail(finder->rail);
				}
				break;
			case 'x':
				if(finder->rail->vehicle != NULL){
					actual=finder->rail->vehicle;
					sim->vehicle = actual;
					if(actual != NULL){
						sim->del();	
						finder->rail->vehicle = NULL;
						sim->vehicle = NULL;
						actual = NULL;
						// provisionalmente tomamos la siguiente loco
						sim->nextloco();	
					}
				}
				break;
			}
			break;
		}
	}
}
