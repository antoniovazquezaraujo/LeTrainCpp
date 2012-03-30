#include <boost/spirit/core.hpp>
#include <boost/spirit/utility/confix.hpp>
#include <iostream>
#include <map>
#include <string>
#include "boost/lambda/bind.hpp"
#include "boost/lambda/lambda.hpp"
#include "boost/any.hpp"
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
using namespace std;
using namespace boost::spirit;
using namespace boost::lambda;
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
	static Logger log;

};
ParserData d;
Logger ParserData::log= Logger::getInstance("ParserData");

////////////////////////////////////////////////

void cleanData(const char * a, const char* b){
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
	LOG_DEBUG(d.log," entrando en moveBulldozer");
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
struct SetBulldozerMode{
	RailPen::RailPenMode mode;
	SetBulldozerMode(RailPen::RailPenMode mode){
		this->mode= mode;
	}
	void operator()(const char * a, const char* b)const{
		d.game->sim.railPen.setMode(mode);
	}
};
struct SetTargetForkDir{
	void operator()(const char * a, const char* b)const{
		d.eventProgram->addMessage(new Message(Message::FORK_DIR, d.targetForkId, d.propertyValue));
		LOG_DEBUG(d.log," agregando mensaje para poner fork objetivo " << d.targetForkId << " valor: " << d.propertyValue);
	}
};
struct SetTargetSemaphoreStatus{
	void operator()(const char * a, const char* b)const{
		LOG_DEBUG(d.log," agregando mensaje para poner semaforo objetivo " << d.targetSemaphoreId << " valor: " << d.propertyValue);
		d.eventProgram->addMessage(new Message(Message::SEMAPHORE, d.targetSemaphoreId, d.propertyValue));
	}
};
struct SetTargetLocomotiveSpeedValue{
	void operator()(const char * a, const char* b)const{
		d.eventProgram->addMessage(new Message(Message::TRAIN_SPEED, d.targetLocomotiveId, d.propertyValue));
	}
};
struct SetEventLocomotiveSpeedValue{
	void operator()(const char * a, const char* b)const{
		d.eventProgram->addMessage(new Message(Message::TRAIN_AT_SENSOR_SPEED, 0, d.propertyValue));
	}
};
struct SetSensorLocomotiveSpeedValue{
	void operator()(const char * a, const char* b)const{
		d.eventProgram->addMessage(new Message(Message::EVENT_TRAIN_SPEED, 0, d.propertyValue));
	}
};

struct SetPropertyValue{
	int value;
	SetPropertyValue(int n){
		this->value = n;
	}
	void operator()(const char * a, const char* b)const{
		d.propertyValue = value;
	}
};
void setTargetFork(int id){
	LOG_DEBUG(d.log," poniendo la id de fork objetivo " << id); 
	d.targetForkId = id;
}
void setTargetSemaphore(int id){
	LOG_DEBUG(d.log," poniendo la id de semaforo objetivo " << id); 
	d.targetSemaphoreId= id;
}
void setTargetLocomotive(int id){
	d.targetLocomotiveId= id;
}
void setSensorLocomotive(const char * a, const char * b){
	d.usingSensorLocomotive = true;
}
void setEventLocomotive(const char * a, const char * b){
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
void showLocomotives(const char * a, const char * b) {
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
void showWagons(const char * a, const char * b){
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
	LOG_DEBUG(d.log," agregando semaforo" << t);
}
void addSensor(int t){
	Sensor * s = new Sensor(t);
	d.game->sim.addSensor(s);
	d.sensor= s;
	LOG_DEBUG(d.log," agregando sensor " << t);
}
void addEventProgram(int t){
	EventProgram * w = new EventProgram();
	d.game->sim.addEventProgram(w);
	d.eventProgram = w;
	LOG_DEBUG(d.log," agregando programa " << t);
}
void addProgramFilter(const char * a, const char * b){
	d.filter = new Filter();
	d.eventProgram->setFilter(d.filter);
	LOG_DEBUG(d.log," agregando filtro " );
}
struct CreateRange{
	int rangeType;
	CreateRange(int rangeType){
		this->rangeType = rangeType;
	}
	void operator()(const char * a, const char* b)const{
		d.rangeType = rangeType;
		d.range = new Range();
	}
};
struct SelectObject{
	int objectType;
	SelectObject(int t){
		this->objectType = t;
	}
	void operator()(const char * a, const char* b)const {
		d.objectType = this->objectType;
	}
};
struct SelectProperty{
	int property;
	SelectProperty(int p){
		this->property = p;
	}
	void operator()(const char * a, const char* b)const{
		d.property = this->property;
	}
};

void addRangeNumber(int value){
	LOG_DEBUG(d.log," agregando numero al rango " << value);
	d.range->addNumber(value);
} 
void addRangePairL(int value){
	d.rangePairL = value;
} 
void addRangePairR(int value){
	d.rangePairR = value;
}
void addRangePair(const char * a, const char * b){
	d.range->addPair(d.rangePairL, d.rangePairR);
}
void addRangeGtCondition(int value){
	d.range->addGtCondition(value);
}
void addRangeLtCondition(int value){
	d.range->addLtCondition(value);
}

struct SetLocomotiveValue {
	void operator()(const char* a, const char*b) const {
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
};
void setFilterRange(const char * a, const char * b){ 
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
struct SetBulldozerValue {
	void operator()(const char* a, const char*b) const {
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
	};
};
struct SetWagonValue {
	void operator()(const char* a, const char*b) const {
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
	};
};
struct SetSemaphoreValue {
	void operator()(const char* a, const char*b) const {
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
				LOG_DEBUG(d.log," abriendo semaforo");
			}else{
				LOG_DEBUG(d.log," cerrando semaforo");
				d.semaphore->close();
			}
			break;
		}
	};
};
struct SetSensorValue {
	void operator()(const char* a, const char*b) const {
		switch(d.property){
		case ROW: 
			d.sensor->setRow(d.propertyValue);
			break;
		case COL: 
			d.sensor->setCol(d.propertyValue);
			break;
		}
	}
};
struct SetSensorRail{
	void operator()(const char* a, const char*b) const {
		LOG_DEBUG(d.log," Poniendo rail al sensor " << d.sensor->getId());
		Point p = d.sensor->getPos();
		Rail * r = d.game->sim.railMap.getRailAt(p.row,p.col);
		if(r){
		   d.sensor->setRail(r);
		}	   
	};
};
struct my_grammar : public grammar <my_grammar> {
	template <typename ScannerT>
	struct definition {
		definition(my_grammar const& self) {
			my_rule 
				= *(command [&cleanData])
				;

			command
				= comment_p("/*","*/") 
				| str_p("show")   >> showing
				| str_p("show")   >> multishowing
				| str_p("add")    >> adding
				| str_p("set")    >> setting
				| str_p("move")   >> moving 
				| str_p("rotate") >> rotating 
				| str_p("delete") >> deleting
				;

			showing	
				= str_p("locomotive") >> uint_p [&showLocomotive]
				| str_p("wagon")      >> uint_p [&showWagon] 
				;

			multishowing	
				= str_p("wagons")      [&showWagons] 
				| str_p("locomotives") [&showLocomotives]
				;

			adding 
				= str_p("locomotive") 
					>> uint_p [&addLocomotive] 
					>> ( *locomotiveSettings [SetLocomotiveValue()] )
				| str_p("wagon")      
					>> uint_p [&addWagon]      
					>> ( *wagonSettings [SetWagonValue()] )
				| str_p("semaphore")      
					>> uint_p [&addSemaphore]      
					>> ( *semaphoreSettings [SetSemaphoreValue()] )
				| str_p("sensor")      
					>> uint_p [&addSensor]      
					>> ( *sensorSettings [SetSensorValue()] ) [SetSensorRail()]
				| str_p("program")    
					>> uint_p [&addEventProgram]    
					>> "{"
					>> ( !programFilters  ) 
					>> ( !programMessages )
					>> "}"
				;

			programFilters
				= str_p("filters") >> str_p(":") [&addProgramFilter] >> +programFilter 
				;

			programFilter
				= str_p("sensor")     [CreateRange(SENSOR)]     >> rangeList [&setFilterRange]
				| str_p("hour")       [CreateRange(HOURS)]      >> rangeList [&setFilterRange] 
				| str_p("minute")     [CreateRange(MINUTES)]    >> rangeList [&setFilterRange]
				| str_p("second")     [CreateRange(SECONDS)]    >> rangeList [&setFilterRange]
				| str_p("locomotive") [CreateRange(LOCOMOTIVE)] >> rangeList [&setFilterRange] 
				| str_p("dir")        [CreateRange(DIR)]        >> rangeList [&setFilterRange] 
				| str_p("speed")      [CreateRange(SPEED)]      >> rangeList [&setFilterRange] 
				;

			rangeList
				= range % str_p(",")
				;

			range
				= uint_p [&addRangeNumber] 
				| (uint_p [&addRangePairL] >> ch_p('-') >> uint_p [&addRangePairR]) [&addRangePair]
				| ch_p('>') >> uint_p [&addRangeGtCondition]
				| ch_p('<') >> uint_p [&addRangeLtCondition]
				;

			programMessages
				= str_p("messages") >> str_p(":") >> +message 
				;

			message	
				= str_p("set") >> settingMessage
				;

			settingMessage
				= str_p("fork")       >> uint_p              [&setTargetFork]       >> ( +dirValue    [SetTargetForkDir()             ])
				| str_p("semaphore")  >> uint_p              [&setTargetSemaphore]  >> ( +statusValue [SetTargetSemaphoreStatus()     ])
				| str_p("locomotive") >> uint_p              [&setTargetLocomotive] >> ( +speedValue  [SetTargetLocomotiveSpeedValue()])
				| str_p("sensor")     >> str_p("locomotive") [&setSensorLocomotive] >> ( +speedValue  [SetSensorLocomotiveSpeedValue()])
				| str_p("event")      >> str_p("locomotive") [&setEventLocomotive]  >> ( +speedValue  [SetEventLocomotiveSpeedValue() ])
				;

			setting 
				= str_p("locomotive") >> uint_p [&getLocomotive] >> ( +locomotiveSettings [SetLocomotiveValue()] )
				| str_p("wagon")      >> uint_p [&getWagon]      >> ( +wagonSettings      [SetWagonValue()]      )
				| str_p("semaphore")  >> uint_p [&getSemaphore]  >> ( +semaphoreSettings  [SetSemaphoreValue()]  )
				| str_p("bulldozer")                             >> ( +bulldozerSettings  [SetBulldozerValue()]  )
				;

			moving
				= str_p("bulldozer") >> int_p [&moveBulldozer]
				;

			rotating	
				= str_p("bulldozer") >> int_p [&rotateBulldozer]
				;

			deleting	
				= "locomotive" >> uint_p [&deleteLocomotive] 
				| "wagon" >> uint_p [&deleteWagon]
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
				= str_p("link") >>  '(' >> uint_p [assign_a(d.propertyValue)] >> ')'
				;

			unlinkValue 
				= str_p("unlink") >> '(' >> uint_p [assign_a(d.propertyValue)] >>')'
				;

			rowValue 
				= str_p("row")  >>'(' >> uint_p [assign_a(d.propertyValue)] >> ')'
				;
			statusValue 
				= str_p("status")  >>'(' >> str_p("open")  [SetPropertyValue(Semaphore::OPEN)] >> ')'
				| str_p("status")  >>'(' >> str_p("closed")[SetPropertyValue(Semaphore::CLOSED)] >> ')'
				;
			modeValue 
				= str_p("mode")  >>'(' >> str_p("moving")  [SetBulldozerMode(RailPen::MOVING)]   >> ')'
				| str_p("mode")  >>'(' >> str_p("painting") [SetBulldozerMode(RailPen::PAINTING)] >> ')'
				;

			colValue 
				= str_p("col")  >>'(' >> uint_p [assign_a(d.propertyValue)] >>')'
				;

			speedValue 
				= str_p("speed")  >>'('  >>uint_p [assign_a(d.propertyValue)] >>')'
				;

			powerValue 
				= str_p("power")  >>'('  >>uint_p  [assign_a(d.propertyValue)] >>')'
				;

			dirValue 
				= str_p("dir")  >>'('  >>uint_p [assign_a(d.propertyValue)] >>')'
				;

			brakesValue 
				= str_p("brakes")  >>'('  >>uint_p [assign_a(d.propertyValue)] >>')'
				;

		}

		rule<ScannerT> 
			my_rule, bulldozerSettings, moving, rotating, modeValue,
			elements, statusValue, command, semaphoreSettings,
			element, range, rangeList, sensorSettings,
			adding ,setting, showing, deleting, multishowing,
			wagonSettings, locomotiveSettings, linkValue ,unlinkValue,
			rowValue ,colValue ,speedValue ,powerValue ,dirValue,
			brakesValue, programFilter, programFilters, programMessages,
			message, settingMessage; 


		rule<ScannerT> const& start() const { 
			return my_rule; 
		}
	};
};

bool parse(string str, Game * game){
	d.game = game;
	d.game->sim =game->sim;
	my_grammar g;
	parse_info<> info = parse(str.c_str(), g, space_p);
	if(info.full){
		return true;
	}else{
		return false;
	}
}
