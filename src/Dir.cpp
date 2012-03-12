#include "Basic.h"
#include "Dir.h"
MAKE_LOGGER(Dir);

char Dir::getAspectChar()const{
	char ret = 0; 
	switch(value){
	case N:
		ret =  '^';
		break;
	case S:
		ret =  'v';
		break;
	case E:
		ret =  '>';
		break;
	case W:
		ret =  '<';
		break;
	case NE:
	case SW:
		ret =  '/';
		break;
	case NW:
	case SE:
		ret =  '\\';
		break;
	default:
		ret = '*';
		break;
	}
	return ret;
}
ostream & operator << (ostream & o, Dir dir){
	o << dir.getAspectChar();
	return o;
}
Dir::Dir(int d ){
	if(d == NO_DIR){
		value = NO_DIR;
	}else if(d < 0){
		value = NUM_DIRS + d;
	}else if (d > SE){
		value = d - NUM_DIRS;
	}else{
		value = d;
	}
}
Dir::Dir(const Dir& d){
	value = d.value;
}
int Dir::getByName(const char * s) {
	if(!strcmp(s, "E"))
		return E;
	if(!strcmp(s, "NE"))
		return NE;
	if(!strcmp(s, "N"))
		return N;
	if(!strcmp(s, "NW"))
		return NW;
	if(!strcmp(s, "W"))
		return W;
	if(!strcmp(s, "SW"))
		return SW;
	if(!strcmp(s, "S"))
		return S;
	if(!strcmp(s, "SE"))
		return SE;
	return NO_DIR;
}
const char * Dir::getName(int value) {
	Dir d(value);
	return d.getName();
}
const char * Dir::getName() const{
	switch(value){
	case E:
		return "E";
		break;
	case NE:
		return "NE";
		break;
	case N:
		return "N";
		break;
	case NW:
		return "NW";
		break;
	case W:
		return "W";
		break;
	case SW:
		return "SW";
		break;
	case S:
		return "S";
		break;
	case SE:
		return "SE";
		break;
	default:
		return "X";
	}
}
int Dir::getValue() const{
	return value;
}
void Dir::setValue(int d){
	if((d >=E)&&(d <= SE)){
		value = d;
	}else{
		value = NO_DIR;
	}
}
void Dir::operator+=(int d){
	value += d;
	if(value < 0){
		value = NUM_DIRS + value ;
	}else if (value > SE){
		value = value - NUM_DIRS;
	}
}
Dir Dir::operator++(int ){
	Dir d(*this);
	value++;
	if(value > SE){
		value = E;
	}
	return d;
}
Dir & Dir::operator++(){
	value++;
	if(value > SE){
		value = E;
	}
	return *this;
}	
Dir Dir::operator--(int ){
	Dir d(*this);
	value--;
	if(value < E){
		value = SE;
	}
	return d;
}
Dir & Dir::operator--(){
	value--;
	if(value < E){
		value = SE;
	}
	return *this;
}	
bool Dir::operator<(Dir d){
	return value< d.value;
}
bool Dir::operator<=(Dir d){
	return value<= d.value;
}
bool Dir::operator==(Dir d){
	return value == d.value;
}
Dir Dir::operator+(Dir d){
	Dir ret(value+d.value);
	return ret;
}
//Distancia angular
int Dir::operator-(Dir d){
	return shortWay(value - d.value);
}
Dir Dir::operator+(int n){
	Dir ret(n+value);
	return ret;
}
// Direccion inversa
Dir Dir::operator-(){
	if(value != NO_DIR){
		return Dir (value+MIDDLE_ANGLE);
	}else{
		return Dir(NO_DIR);
	}
}
int Dir::shortWay(int angle){
	int absValue = abs(angle);
	if(absValue > MIDDLE_ANGLE){
		return (NUM_DIRS - absValue) * -1;
	}else{
		return angle;
	}
}
bool Dir::isCurve(Dir d){
	int distance = abs(d.value - value);
	return  ((distance == MIN_CURVE_ANGLE) || (distance == MAX_CURVE_ANGLE));
}
bool Dir::isStraight(Dir d){
	return (abs(d.value - value) == MIDDLE_ANGLE);
}
