#include <iostream>
#include "graphics.h" 
using namespace std;
void loop();
void processKeys();
void move();
void repaint();
class Vehicle{
	public:
		Vehicle();
		void move();
		void paint();
		void calculateMove();
		void newCalculateMove();
		float power;
		float impulse;
		float speed;
		int turns;
		float mass;
		int position;
		float friction;
		int maxTurns;
};

Vehicle v;
char espacios[80];
Vehicle::Vehicle():
	power(0.0), 
	impulse(0.0), 
	speed(0.0), 
	mass(50.0), 
	position(0), 
	friction(0.1), 
	maxTurns(10),
	turns(0){ 
		turns = maxTurns;
}
void Vehicle::move(){
	position++;
	//printf("%c", '\007');
}

void Vehicle::calculateMove(){
	impulse += power;
	if(impulse > 0 ){
		impulse-=(mass * friction/10);
		if(impulse <0) {
			impulse = 0;
		}
		speed = impulse/mass;
		if(turns > 0){
			turns--;
		}else{
			move();
			if((int)speed >0){
				turns = (int)(maxTurns * (1.0/speed));
			}else{
				turns = maxTurns;
			}
		}
	}
}
void Vehicle::paint(){
	position %= 80;
	g_putstr(10,0, espacios, 0,0);
	g_putc(10,position, '*', 5,0);
	mvprintw(15,0, "Speed:%f\nPower:%f\nImpulse:%f\nTurns:%d\nMass:%f\nFriction:%f\nMaxTurns:%d\n", speed, power, impulse, turns, mass, friction, maxTurns);
}

void loop(){
	while(1){
		processKeys();
		move();
		repaint();
	}
}
void processKeys(){
	char c = g_getkey();
	if(c != -1){
		switch (c){
			case 'p': v.power--;break;
			case 'P': v.power++;break;
			case 'i': v.impulse--;break;
			case 'I': v.impulse++;break;
			case 't': v.turns--;break;
			case 'T': v.turns++;break;
			case 'a': v.speed--;break;
			case 'A': v.speed++;break;
			case 'm': v.mass--;break;
			case 'M': v.mass++;break;
			case 'f': v.friction-=0.1;break;
			case 'F': v.friction+=0.1;break;
			case 'x': v.maxTurns-=10;break;
			case 'X': v.maxTurns+=10;break;
		}
	}
}
void move(){
	v.calculateMove();
}
void repaint(){
	v.paint();
	g_repaint();
}
int main4(){
	for(int n=0;n<80;n++){
		espacios[n] = ' ';
	}
	g_init();
	loop();
	g_reset();
}
