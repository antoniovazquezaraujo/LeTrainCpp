#include "Filter.h"
#include "Range.h"
#include "Event.h"

bool Filter::accept(Event * event){
	

	if(sensorId        && !sensorId       ->accept(event->getSensorId()))       return false;
	if(hour            && !hour           ->accept(event->getHour()))           return false;
	if(minute          && !minute         ->accept(event->getMinute()))         return false;
	if(second          && !second         ->accept(event->getSecond()))         return false;
	if(locomotiveId    && !locomotiveId   ->accept(event->getLocomotiveId()))   return false;
	if(locomotiveSpeed && !locomotiveSpeed->accept(event->getLocomotiveSpeed()))return false;
	if(locomotiveDir   && !locomotiveDir  ->accept(event->getLocomotiveDir()))  return false;

	return true;
}
Filter::Filter(Range * sensorId, Range * hour, Range * minute, Range *second, 
		Range * locomotiveId, Range * locomotiveSpeed, Range * locomotiveDir)
	:sensorId(sensorId), hour(hour), minute(minute), second(second),
	locomotiveId(locomotiveId), locomotiveSpeed(locomotiveSpeed), locomotiveDir(locomotiveDir){

}	
Filter::Filter(const Filter & f) 
	:sensorId(f.sensorId), hour(f.hour), minute(f.minute), second(f.second),
	locomotiveId(f.locomotiveId), locomotiveSpeed(f.locomotiveSpeed), locomotiveDir(f.locomotiveDir){
}
Filter::Filter()
	:sensorId(0), hour(0), minute(0), second(0),
	locomotiveId(0), locomotiveSpeed(0), locomotiveDir(0){
}

string Filter::str(){
	stringstream s("");
	s << "Sensor: " <<     string(sensorId?sensorId->str():"") << "\n";
	s << "Hour: "   <<     string(hour?hour->str():"") << "\n";
	s << "Minute: " <<     string(minute?minute->str():"") << "\n";
	s << "Second: " <<     string(second?second->str():"") << "\n";
	s << "Locomotive: " << string(locomotiveId?locomotiveId->str():"") << "\n";
	s << "Speed: " <<      string(locomotiveSpeed?locomotiveSpeed->str():"") << "\n";
	s << "Direction: " <<  string(locomotiveDir?locomotiveDir->str():"") << "\n";
	return s.str();
}
Range * Filter::getSensorId(){
	return sensorId;
}

void Filter::setSensorId(Range* r){
	this->sensorId = r;
}

Range* Filter::getHour(){
	return hour ;
}

void Filter::setHour(Range* r){
	this->hour= r;
}

Range* Filter::getMinute(){
	return  minute;
}

void Filter::setMinute(Range* r){
	this->minute= r;
}

Range* Filter::getSecond(){
	return  second;
}

void Filter::setSecond(Range* r){
	this->second= r;
}

Range* Filter::getLocomotiveId(){
	return  locomotiveId;
}

void Filter::setLocomotiveId(Range* r){
	this->locomotiveId= r;
}

Range* Filter::getLocomotiveSpeed(){
	return  locomotiveSpeed;
}

void Filter::setLocomotiveSpeed(Range* r){
	this->locomotiveSpeed= r;
}

Range* Filter::getLocomotiveDir(){
	return  locomotiveDir;
}

void Filter::setLocomotiveDir(Range* r){
	this->locomotiveDir= r;
}

