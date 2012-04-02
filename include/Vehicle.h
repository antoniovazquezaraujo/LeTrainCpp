#ifndef VEHICLE_H
#define VEHICLE_H
#include "Basic.h"
#include "Point.h"
#include "Dir.h"
class Vehicle{
public:
	Vehicle  (Aspect * aspect);
	~Vehicle ();
	void rotate(int angle = 1);
	void rotateLeft(int angle = 1);
	void rotateRight(int angle = -1);
	Point  	getPos()const;
	Dir 	getDir()const;
	Aspect* getAspect();
	void setPos(Point pos);
	void setRow(int row);
	void setCol(int col);
	void setDir(Dir dir);
	virtual void paint(Window * g)=0;
	virtual void erase(Window * g)=0;
protected:
	Aspect * aspect;
	Point pos;
	Dir dir;
	friend ostream & operator << (ostream & o, Vehicle & v);
	
};
#endif
