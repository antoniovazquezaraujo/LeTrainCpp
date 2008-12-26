#ifndef COMMANDER_H
#define COMMANDER_H
#include "Basic.h"
class Commander;
typedef void (Commander::*Method)(void); 
struct Action{ 
	int mode;
	char key;
	string name;
	Method method;
};

class Commander{
public:
	enum CommandMode {
		DRIVERS_MODE, 
		POINTS_MODE,
		TRAINS_MODE,
		RAILS_MODE,
		EVENTS_MODE,	
		RAILPEN_MODE
	};
	typedef map<char, Action> ActionsMap;
	typedef map< char, CommandMode> ModesMap;
	Commander(Game * game);
	~Commander();
	void paint();
	void erase();
	void doAction(char key);
private:
	void onSetDriversMode();	
	void onSetPointsMode();	
	void onSetTrainsMode();	
	void onSetRailsMode();	
	void onSetRailPenMode();	
	void onSetEventsMode();	
	void toggleRailPenPainting();
	void rotateLeftRailPen();
	void rotateRightRailPen();
	void railPenForward();
	void railPenBackwards();
	void railPenPlaceFinder();
	void railPenLoadProgram();

	void finderChangeFork();
	void finderForward();
	void finderReverse();
	void finderUnlink();
	void finderLink();
	void finderAddWagon();
	void finderAddTrain();
	void finderAddSensor();
	void finderDeleteVehicle();
	void finderIncFork();
	void finderDecFork();

	Action getAction(int mode, char key);
	bool isValidAction(int mode, char key);
	void registerAction(string name, Method method, int mode, char key);
	void registerMode(CommandMode mode, char key);
	const char * getModeName(int mode);
	void setMode(char key);
	CommandMode getMode();

	map< int,  ActionsMap>  maps;
	map< char, CommandMode> modes;

	CommandMode mode;
	Game * game;
	CommanderView * view;
	static Logger log;
	friend class CommanderView;
		
};
#endif
