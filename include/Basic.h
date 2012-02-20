#ifndef BASIC_H
#define BASIC_H
#include <cassert> 
#include <cstdlib> 
#include <cstring>
#include <string>
#include <fstream> 
#include <sstream> 
#include <iostream> 
#include <iomanip> 
#include <ncurses.h>
#include <iterator>
#include <vector>
#include <list>
#include <map>

#include "Log.h"

using namespace std;
class Dir;
class Point;
class DirEnv;
class RailEnv;
class ForkDirSelector;
class Vehicle;
class RailMap;
class RailPen;
class Window;
class TerminalWindow;
class Commander;
class Game;
class Sim;
class SimView;
class CommanderView;
class Link;
class RailVehicle;
class TrainIterator;
class Finder;
class Train;
class Locomotive;
class LocomotiveAspect;
class Wagon;
class WagonAspect;
class Event;
class EventProgram;
class ProgramEditor;
class Sensor;
class Message;    
class Semaphore;   
class Range;   
class Filter;   

class Aspect;
class StraightAspect;
class ForkAspect;
class CurveAspect;
class CrossAspect;
class FinderAspect;

class Rail;
class StraightRail;
class ForkRail;
class CurveRail;
class CrossRail;

const int NUM_DIRS = 8;
const int NO_DIR = 999;
const int RAIL_MAP_ROWS = 25;
const int RAIL_MAP_COLS = 80;

//crea un logger para cada clase. Agregarlo al principio de cada fichero
// con MAKE_LOGGER(NombreDeLaClase); (ojo, poner el ; )
#define MAKE_LOGGER(className) Logger className::log = Logger::getInstance(# className)

#define LOG_DEBUG LOG4CPLUS_DEBUG
#define LOG_INFO  LOG4CPLUS_INFO
#define LOG_WARN  LOG4CPLUS_WARN
#define LOG_ERROR LOG4CPLUS_ERROR
#define LOG_FATAL LOG4CPLUS_FATAL
//counterclockwise from east to southweast
/*
enum {
	NW =3, N =2, NE =1,
	W  =4,       E  =0,
	SW =5, S =6, SE =7
};
*/
enum{
	MIDDLE_ANGLE   =NUM_DIRS/2, 
	MIN_CURVE_ANGLE=MIDDLE_ANGLE-1, 
	MAX_CURVE_ANGLE=MIDDLE_ANGLE+1 
};

#endif
