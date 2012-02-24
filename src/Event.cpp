#include "Event.h"
#include "Dir.h"
Event::Event(int sensorId, int hour, int minute, int second, 
		int locomotiveId, int locomotiveSpeed, int locomotiveDir)
	:sensorId(sensorId), hour(hour), minute(minute), second(second), 
	locomotiveId(locomotiveId), locomotiveSpeed(locomotiveSpeed),  
	locomotiveDir(locomotiveDir){
}
Event::Event(const Event & e)
	:sensorId(e.sensorId), 
	hour(e.hour), 
	minute(e.minute), 
	second(e.second), 
	locomotiveId(e.locomotiveId), 
	locomotiveSpeed(e.locomotiveSpeed),  
	locomotiveDir(e.locomotiveDir){
}
string Event::str(){
	stringstream s("");
	s 
	<< "Sensor: "     << sensorId        << endl
	<< "Hour: "       << hour            << endl
	<< "Minute: "     << minute          << endl
	<< "Second: "     << second          << endl
	<< "Locomotive: " << locomotiveId    << endl
	<< "Speed: "      << locomotiveSpeed << endl
	<< "Direction: "  << locomotiveDir   << endl;
	return s.str();
}
Event::Event()
	:sensorId(0), 
	hour(0), 
	minute(0), 
	second(0), 
	locomotiveId(0), 
	locomotiveSpeed(0),  
	locomotiveDir(Dir::NO_DIR){
}
int Event::getSensorId(){
	return sensorId;
}
int Event::getHour(){
	return hour;
}
int Event::getMinute(){
	return minute;
}
int Event::getSecond(){
	return second;
}
int Event::getLocomotiveId(){
	return locomotiveId;
}
int Event::getLocomotiveSpeed(){
	return locomotiveSpeed;
}
int Event::getLocomotiveDir(){
	return locomotiveDir;
}
void Event::setSensorId(int n){
	sensorId = n;
}
void Event::setHour(int n){
	hour=n;
}
void Event::setMinute(int n){
	minute=n;
}
void Event::setSecond(int n){
	second = n;
}
void Event::setLocomotiveId(int n){
	locomotiveId=n;
}
void Event::setLocomotiveSpeed(int n){
	locomotiveSpeed=n;
}
void Event::setLocomotiveDir(int n){
	locomotiveDir = n;
}

