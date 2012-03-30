#include "Basic.h"
#include "SimView.h"
#include "Window.h"
#include "Vehicle.h"
#include "Sim.h"
#include "Locomotive.h"
#include "Train.h"
#include "Semaphore.h"
#include "Wagon.h"
#include "RailMap.h"
#include "RailPen.h"
#include "Finder.h"
#include "Sensor.h"
MAKE_LOGGER(SimView);

SimView::SimView(Sim * sim)
	:sim(sim), vehicleToFollow(0){
	int screenRows = Window::getScreenRows();
	int screenCols = Window::getScreenCols();
	int commandCols = screenCols * 20/100;
	int commandRows = screenRows * 20/100;
	int mapCols = screenCols - commandCols;
	int mapRows = screenRows - commandRows;

	window = new Window(0, 0, mapCols, mapRows);
	window->setTitle("Map");

	recalcWindowShift();
}

SimView::~SimView(){
	delete window; 
} 

void SimView::recalcWindowShift(){
	int hLimit = (window->getWidth()/2);
	int vLimit = (window->getHeight()/2);
	int hShift=hLimit;
	int vShift=vLimit;
	if(vehicleToFollow){
		hShift = vehicleToFollow->getPos().col;
		vShift = vehicleToFollow->getPos().row;
	}
	int perimeter= 2;
	if( (hShift > pos.col+ hLimit-perimeter) ||
		(hShift < pos.col- hLimit+perimeter)){
		pos.col = hShift;
		window->shift(Window::HORIZONTAL_SHIFT, hShift);
	}
	if( (vShift > pos.row+ vLimit-perimeter) ||
		(vShift < pos.row- vLimit+perimeter)){
		pos.row = vShift;
		window->shift(Window::VERTICAL_SHIFT,vShift);
	}
}
void SimView::followVehicle(Vehicle * v){
	vehicleToFollow = v;
}
void SimView::setPos(Point p){
	pos = p;
}
Point SimView::getPos(){
	return pos;
}
void SimView::erase(){
	sim->railMap.erase(window);
	sim->railPen.erase(window);
	sim->getFinder()->erase(window);
}
void SimView::paint(){
	recalcWindowShift();
	sim->railMap.paint(window);
	sim->railPen.paint(window);

	for(auto semaphore : sim->getSemaphores()){
		semaphore->paint(window);
	}
	for(auto sensor: sim->getSensors()){
		sensor->paint(window);
	}
	for(auto train: sim->getTrains()){
		train->paint(window);
	}

	sim->getFinder()->paint(window);
	window->repaint();
}
