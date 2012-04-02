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
#include <deque>
#include <array>
#include <list>
#include <map>
#include <algorithm>


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

const int RAIL_MAP_ROWS = 25;
const int RAIL_MAP_COLS = 80;

const int FRONT=0;
const int BACK=1;

#endif
