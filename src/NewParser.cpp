#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/repository/include/qi_confix.hpp>

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/container/vector/vector_fwd.hpp>
#include <boost/fusion/include/vector_fwd.hpp>

#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <algorithm>
#include "Sim.h" 
#include "Locomotive.h" 
#include "Train.h" 
#include "Wagon.h" 
#include "Filter.h" 
#include "Message.h" 
#include "Event.h" 
#include "EventProgram.h" 
#include "Rail.h" 
#include "RailMap.h" 
#include "RailPen.h" 
#include "Semaphore.h" 
#include "ForkRail.h" 
#include "Sensor.h" 
#include "Game.h" 
#include "ProgramEditor.h" 
///////////////////////////////////////////////////////////////////////////////
using namespace   std;
using namespace   boost::spirit;
using namespace   boost::fusion;
using ascii::char_;
/*
namespace qins   =boost::spirit::qi;
namespace asciins=boost::spirit::ascii;
namespace repo   =boost::spirit::repository;
using boost::spirit::eol;
using boost::spirit::int_;
using asciins::char_;
using asciins::string;
using qins::eps;
using qins::lit;
using qins::_val;
using qins::_1;
using repo::confix;
*/

enum {ROW, COL, SPEED, POWER, BRAKES, DIR, LINK, UNLINK, SENSOR, HOURS, MINUTES,
	SECONDS, LOCOMOTIVE, WAGON, MODE, STATUS
};

struct ParserData{
	Game * game;
	Locomotive * locomotive;
	Wagon * wagon;
	EventProgram * eventProgram;
	Filter * filter;
	Message * message;
	Range * range;
	Sensor * sensor;
	Semaphore * semaphore;
	int property;
	int propertyValue;
	int rangePairL, rangePairR;
	int rangeType;
	int objectType;
	int targetForkId;
	int targetSemaphoreId;
	int targetLocomotiveId;
	bool usingEventLocomotive;
	bool usingSensorLocomotive;

};
ParserData d;

////////////////////////////////////////////////

//void cleanData(const char * a, const char* b){
void cleanData(){
	d.property = 0;
	d.propertyValue = 0;
	d.locomotive=0;
	d.wagon = 0;
	d.semaphore = 0;
	d.eventProgram = 0;
	d.filter = 0;
	d.message = 0;
	d.sensor = 0;
	d.range = 0;
	d.rangePairL = 0;
	d.rangePairR = 0;
	d.rangeType = 0;
	d.objectType = 0;
	d.targetForkId = 0;
	d.targetSemaphoreId = 0;
	d.targetLocomotiveId = 0;
	d.usingEventLocomotive = false;
	d.usingSensorLocomotive = false;
}
void moveBulldozer(int dist){
	
	for(int n=0; n<dist;n++){
		//	d.game->sim->getRailPen()->move(1, d.game->sim);
		Point p = d.game->sim.railPen.getPos();
		if(d.game->sim.railPen.getMode() == RailPen::PAINTING){
			Rail * newRail = d.game->sim.railPen.makeNewRail(d.game->sim.railMap.getRailAt(p.row, p.col));
			d.game->sim.railMap.setRail(p.row, p.col, newRail);

			ForkRail *rf = dynamic_cast<ForkRail *>(newRail);
			if(rf){
				d.game->sim.addFork(rf);
			}

			Point lastPoint = d.game->sim.railPen.getPos();
			Dir lastDir = d.game->sim.railPen.getLastDir();
			Dir backDir = -lastDir; 
			lastPoint.move(backDir);
			Rail * lastRail = d.game->sim.railMap.getRailAt(lastPoint.row, lastPoint.col);
			if(lastRail){
				newRail->linkRailAt(backDir, lastRail);
			}
		}
		d.game->sim.railPen.forward();
	}
}
void rotateBulldozer(int angle){
	d.game->sim.railPen.rotate(angle);
}
void setPropertyValue(int value){
	d.propertyValue=value;
}
struct SetBulldozerMode{
	RailPen::RailPenMode mode;
	SetBulldozerMode(RailPen::RailPenMode mode){
		this->mode= mode;
	}
	void operator()()const{
		d.game->sim.railPen.setMode(mode);
	}
};
struct SetTargetForkDir{
	void operator()(int n)const{
		d.eventProgram->addMessage(new Message(Message::FORK_DIR, d.targetForkId, d.propertyValue));
		
	}
};
struct SetTargetSemaphoreStatus{
	void operator()()const{
		
		d.eventProgram->addMessage(new Message(Message::SEMAPHORE, d.targetSemaphoreId, d.propertyValue));
	}
};
struct SetTargetLocomotiveSpeedValue{
	void operator()()const{
		d.eventProgram->addMessage(new Message(Message::TRAIN_SPEED, d.targetLocomotiveId, d.propertyValue));
	}
};
struct SetEventLocomotiveSpeedValue{
	void operator()()const{
		d.eventProgram->addMessage(new Message(Message::TRAIN_AT_SENSOR_SPEED, 0, d.propertyValue));
	}
};
struct SetSensorLocomotiveSpeedValue{
	void operator()()const{
		d.eventProgram->addMessage(new Message(Message::EVENT_TRAIN_SPEED, 0, d.propertyValue));
	}
};

struct SetPropertyValue{
	int value;
	SetPropertyValue(int n){
		this->value = n;
	}
	void operator()()const{
		d.propertyValue = value;
	}
};
void setTargetFork(int id){
	
	d.targetForkId = id;
}
void setTargetSemaphore(int id){
	
	d.targetSemaphoreId= id;
}
void setTargetLocomotive(int id){
	d.targetLocomotiveId= id;
}
void setSensorLocomotive(){
	d.usingSensorLocomotive = true;
}
void setEventLocomotive(){
	d.usingEventLocomotive = true;
}
void deleteLocomotive(int t){
	//d.game->sim.removeLocomotive(t);
}
void deleteWagon(int t){
	//d.game->sim.removeWagon(t);
}

void getLocomotive(int n){
	//d.locomotive = d.game->sim.getLocomotive(n);
}
void getWagon(int n){
	//d.wagon = d.game->sim.getWagon(n);
}
void getSemaphore(int n){
	d.semaphore= d.game->sim.getSemaphore(n);
}
void showLocomotives() {
	/*
	for(auto l:d.game->sim.getLocomotives()){
		cout << l << endl;
	}
	*/
}
void showLocomotive(int n){
	/*
	Locomotive * l = d.game->sim.getLocomotive(n);
	cout << *(l) << endl;
	*/
}
void showWagon(int n){
	/*
	Wagon * l = d.game->sim.getWagon(n);
	cout << *(l) << endl;
	*/
}
void showWagons(){
	/*
	for(auto w:d.game->sim.getWagons()){
		cout << w << endl;
	}
	*/
}
void addLocomotive(int t){
	Locomotive * locomotive = new Locomotive; 
	Train * train = new Train;
	train->addVehicle(0,locomotive);
	d.game->sim.addTrain(train);
	Point p = locomotive->getPos();
	Rail * r = d.game->sim.railMap.getRailAt(p.row,p.col);
	if(r) locomotive->gotoRail(r);
	d.locomotive = locomotive;
}
void addWagon(int t){
	Wagon * w = new Wagon();
	Train * train = new Train;
	d.game->sim.addTrain(train);
	//d.game->sim.addWagon(w);
	d.wagon = w;
}
void addSemaphore(int t){
	Semaphore* s = new Semaphore(t);
	d.game->sim.addSemaphore(s);
	d.semaphore= s;
	
}
void addSensor(int t){
	Sensor * s = new Sensor(t);
	d.game->sim.addSensor(s);
	d.sensor= s;
	
}
void addEventProgram(int t){
	EventProgram * w = new EventProgram();
	d.game->sim.addEventProgram(w);
	d.eventProgram = w;
	
}
void addProgramFilter(){
	d.filter = new Filter();
	d.eventProgram->setFilter(d.filter);
	
}
struct CreateRange{
	int rangeType;
	CreateRange(int rangeType){
		this->rangeType = rangeType;
	}
	void operator()(qi::unused_type, qi::unused_type) const{
		d.rangeType = rangeType;
		d.range = new Range();
	}
};
struct SelectObject{
	int objectType;
	SelectObject(int t){
		this->objectType = t;
	}
	void operator()()const {
		d.objectType = this->objectType;
	}
};
struct SelectProperty{
	int property;
	SelectProperty(int p){
		this->property = p;
	}
	void operator()()const{
		d.property = this->property;
	}
};

void addRangeNumber(int value){
	
	d.range->addNumber(value);
} 
void addRangePairL(int value){
	d.rangePairL = value;
} 
void addRangePairR(int value){
	d.rangePairR = value;
}
void addRangePair(boost::fusion::vector<int, char,int> datos){
	d.range->addPair(d.rangePairL, d.rangePairR);
}
void addRangeGtCondition(int value){
	d.range->addGtCondition(value);
}
void addRangeLtCondition(int value){
	d.range->addLtCondition(value);
}

void setLocomotiveValue() {
	Point p;
	Rail* r=0;
	switch(d.property){
	case ROW: 
		d.locomotive->setRow(d.propertyValue);
		p = d.locomotive->getPos();
		r = d.game->sim.railMap.getRailAt(p.row,p.col);
		if(r) d.locomotive->gotoRail(r);
		break;
	case COL: 
		d.locomotive->setCol(d.propertyValue);
		p = d.locomotive->getPos();
		r = d.game->sim.railMap.getRailAt(p.row,p.col);
		if(r) d.locomotive->gotoRail(r);
		break;
	case SPEED: 
		d.locomotive->setSpeed(d.propertyValue);
		break;
	case BRAKES: 
		d.locomotive->setBrakes(d.propertyValue);
		break;
	case DIR: 
		d.locomotive->setDir(d.propertyValue);
		break;
	case POWER: 
		d.locomotive->setPower(d.propertyValue);
		break;
	}
}
void setFilterRange(){ 
	switch(d.rangeType){
	case SENSOR:     
		d.filter->setSensorId(d.range);
		break;
	case HOURS:       
		d.filter->setHour(d.range);
		break;
	case MINUTES:     
		d.filter->setMinute(d.range);
		break;
	case SECONDS:     
		d.filter->setSecond(d.range);
		break;
	case LOCOMOTIVE: 
		d.filter->setLocomotiveId(d.range);
		break;
	case DIR:        
		d.filter->setLocomotiveDir(d.range);
		break;
	case SPEED:      
		d.filter->setLocomotiveSpeed(d.range);
		break;
	}
}
void setBulldozerValue() {
	switch(d.property){
	case ROW: 
		d.game->sim.railPen.setRow(d.propertyValue);
		break;
	case COL: 
		d.game->sim.railPen.setCol(d.propertyValue);
		break;
	case DIR: 
		d.game->sim.railPen.setDir(d.propertyValue);
		break;
	}
}
void setWagonValue() {
	Point p;
	Rail* r=0;
	switch(d.property){
	case ROW: 
		d.wagon->setRow(d.propertyValue);
		p = d.wagon->getPos();
		r = d.game->sim.railMap.getRailAt(p.row,p.col);
		if(r) d.wagon->gotoRail(r);
		break;
	case COL: 
		d.wagon->setCol(d.propertyValue);
		p = d.wagon->getPos();
		r = d.game->sim.railMap.getRailAt(p.row,p.col);
		if(r) d.wagon->gotoRail(r);
		break;
	case SPEED: 
		d.wagon->setSpeed(d.propertyValue);
		break;
	case BRAKES: 
		d.wagon->setBrakes(d.propertyValue);
		break;
	case DIR: 
		d.wagon->setDir(d.propertyValue);
		break;
	}
}
void setSemaphoreValue() {
	switch(d.property){
	case ROW: 
		d.semaphore->setRow(d.propertyValue);
		break;
	case COL: 
		d.semaphore->setCol(d.propertyValue);
		break;
	case STATUS:
		if(d.propertyValue == Semaphore::OPEN){
			d.semaphore->open();

		}else{

			d.semaphore->close();
		}
		break;
	}
}
void setSensorValue () {
	switch(d.property){
	case ROW: 
		d.sensor->setRow(d.propertyValue);
		break;
	case COL: 
		d.sensor->setCol(d.propertyValue);
		break;
	}
}
void setSensorRail() {

	Point p = d.sensor->getPos();
	Rail * r = d.game->sim.railMap.getRailAt(p.row,p.col);
	if(r){
		d.sensor->setRail(r);
	}	   
}
template <typename Iterator>
struct LetrainGrammar : qi::grammar <Iterator, string()> {
	LetrainGrammar()
		:LetrainGrammar::base_type(first){
		first	
			= *(command [&cleanData])
			;
		command
			= repository::confix("//", eol)[*(char_ - eol)] 
			| lit("show")   >> showing
			| lit("show")   >> multishowing
			| lit("add")    >> adding
			| lit("set")    >> setting
			| lit("move")   >> moving 
			| lit("rotate") >> rotating 
			| lit("delete") >> deleting
			;

		showing	
			= lit("locomotive") >> int_ [&showLocomotive]
			| lit("wagon")      >> int_ [&showWagon] 
			;

		multishowing	
			= lit("wagons")      [&showWagons] 
			| lit("locomotives") [&showLocomotives]
			;

		adding 
			= lit("locomotive") 
				>> int_ [&addLocomotive] 
				>> ( *locomotiveSettings [&setLocomotiveValue] )
			| lit("wagon")      
				>> int_ [&addWagon]      
				>> ( *wagonSettings [&setWagonValue] )
			| lit("semaphore")      
				>> int_ [&addSemaphore]      
				>> ( *semaphoreSettings [&setSemaphoreValue] )
			| lit("sensor")      
				>> int_ [&addSensor]      
				>> ( *sensorSettings [&setSensorValue] ) [&setSensorRail]
			| lit("program")    
				>> int_ [&addEventProgram]    
				>> "{"
				>> ( !programFilters  ) 
				>> ( !programMessages )
				>> "}"
			;

		programFilters
			= lit("filters") >> char_(':') [&addProgramFilter] >> +programFilter 
			;

		programFilter
			= lit("sensor")     [CreateRange(SENSOR)]     >> rangeList [&setFilterRange]
			| lit("hour")       [CreateRange(HOURS)]      >> rangeList [&setFilterRange] 
			| lit("minute")     [CreateRange(MINUTES)]    >> rangeList [&setFilterRange]
			| lit("second")     [CreateRange(SECONDS)]    >> rangeList [&setFilterRange]
			| lit("locomotive") [CreateRange(LOCOMOTIVE)] >> rangeList [&setFilterRange] 
			| lit("dir")        [CreateRange(DIR)]        >> rangeList [&setFilterRange] 
			| lit("speed")      [CreateRange(SPEED)]      >> rangeList [&setFilterRange] 
			;

		rangeList
			= range % char_(",")
			;

		range
			= int_ [&addRangeNumber] 
			| (int_ [&addRangePairL] >> char_('-') >> int_ [&addRangePairR])[&addRangePair] 
			| char_('>') >> int_ [&addRangeGtCondition]
			| char_('<') >> int_ [&addRangeLtCondition]
			;

		programMessages
			= lit("messages") >> char_(':') >> +message 
			;

		message	
			= lit("set") >> settingMessage
			;

		settingMessage
			= lit("fork")       >> int_              [&setTargetFork]       >> ( +dirValue    [SetTargetForkDir()             ])
			| lit("semaphore")  >> int_              [&setTargetSemaphore]  >> ( +statusValue [SetTargetSemaphoreStatus()     ])
			| lit("locomotive") >> int_              [&setTargetLocomotive] >> ( +speedValue  [SetTargetLocomotiveSpeedValue()])
			| lit("sensor")     >> lit("locomotive") [&setSensorLocomotive] >> ( +speedValue  [SetSensorLocomotiveSpeedValue()])
			| lit("event")      >> lit("locomotive") [&setEventLocomotive]  >> ( +speedValue  [SetEventLocomotiveSpeedValue() ])
			;

		setting 
			= lit("locomotive") >> int_ [&getLocomotive] >> ( +locomotiveSettings [&setLocomotiveValue] )
			| lit("wagon")      >> int_ [&getWagon]      >> ( +wagonSettings      [&setWagonValue]      )
			| lit("semaphore")  >> int_ [&getSemaphore]  >> ( +semaphoreSettings  [&setSemaphoreValue]  )
			| lit("bulldozer")                           >> ( +bulldozerSettings  [&setBulldozerValue]  )
			;

		moving
			= lit("bulldozer") >> int_ [&moveBulldozer]
			;

		rotating	
			= lit("bulldozer") >> int_ [&rotateBulldozer]
			;

		deleting	
			= lit("locomotive") >> int_ [&deleteLocomotive] 
			| lit( "wagon")     >> int_ [&deleteWagon]
			;

		locomotiveSettings
			= rowValue    [SelectProperty( ROW)]
			| colValue    [SelectProperty( COL)]
			| speedValue  [SelectProperty( SPEED)]
			| powerValue  [SelectProperty( POWER)]
			| brakesValue [SelectProperty( BRAKES)]
			| dirValue    [SelectProperty( DIR)]
			| linkValue   [SelectProperty( LINK)]
			| unlinkValue [SelectProperty( UNLINK)]
			;

		wagonSettings
			= rowValue    [SelectProperty( ROW)]
			| colValue    [SelectProperty( COL)]
			| speedValue  [SelectProperty( SPEED)]
			| brakesValue [SelectProperty( BRAKES)]
			| dirValue    [SelectProperty( DIR)]
			| linkValue   [SelectProperty( LINK)]
			| unlinkValue [SelectProperty( UNLINK)]
			;

		semaphoreSettings
			= rowValue       [SelectProperty( ROW)]
			| colValue       [SelectProperty( COL)]
			| statusValue    [SelectProperty(STATUS)]
			;

		bulldozerSettings
			= rowValue    [SelectProperty( ROW)]
			| colValue    [SelectProperty( COL)]
			| dirValue    [SelectProperty( DIR)]
			| modeValue   [SelectProperty( MODE)]
			;
		sensorSettings
			= rowValue    [SelectProperty( ROW)]
			| colValue    [SelectProperty( COL)]
			;

		linkValue 
			= lit("link") >>  char_('(') >> int_ [&setPropertyValue] >> ')'
			;

		unlinkValue 
			= lit("unlink") >> char_('(') >> int_ [&setPropertyValue] >>')'
			;

		rowValue 
			= lit("row")  >> char_('(') >> int_ [&setPropertyValue] >> ')'
			;
		statusValue 
			= lit("status")  >>char_('(') >> lit("open")  [SetPropertyValue(Semaphore::OPEN)]   >> ')'
			| lit("status")  >>char_('(') >> lit("closed")[SetPropertyValue(Semaphore::CLOSED)] >> ')'
			;
		modeValue 
			= lit("mode")  >>char_('(') >> lit("moving")  [SetBulldozerMode(RailPen::MOVING)]   >> ')'
			| lit("mode")  >>char_('(') >> lit("painting") [SetBulldozerMode(RailPen::PAINTING)] >> ')'
			;

		colValue 
			= lit("col")  >>'(' >> int_ [&setPropertyValue] >>')'
			;

		speedValue 
			= lit("speed")  >>char_('(')  >>int_ [&setPropertyValue] >>')'
			;

		powerValue 
			= lit("power")  >>char_('(')  >>int_ [&setPropertyValue] >>')'
			;

		dirValue 
			= lit("dir")    >>char_('(')  >>int_ [&setPropertyValue] >>')'
			;

		brakesValue 
			= lit("brakes") >>char_('(')  >>int_ [&setPropertyValue] >>')'
			;

	}

	qi::rule<Iterator, boost::fusion::vector<int> >  rangeList;

	qi::rule<Iterator, int()> dirValue; 

	qi::rule<Iterator, string()> 
	first, moving, rotating, modeValue,
	elements, statusValue, command, element, range, 
	adding ,setting, showing, deleting, multishowing,
	linkValue ,unlinkValue,
	rowValue ,colValue ,speedValue ,powerValue ,
	brakesValue, programFilter, programFilters, programMessages,
	message, settingMessage; 

	qi::rule<Iterator> 
	bulldozerSettings, semaphoreSettings, sensorSettings,
	locomotiveSettings, wagonSettings;

};

bool parse(std::string str, Game * game){
	d.game = game;
	d.game->sim =game->sim;
	typedef std::string::iterator iterator_type;
	typedef LetrainGrammar<iterator_type> Grammar;

	Grammar g;
	std::string::iterator beg = str.begin();
	if(qi::parse(beg, str.end(), g)){
		return true;
	}else{
		return false;
	}
}
