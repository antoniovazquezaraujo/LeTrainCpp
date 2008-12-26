#ifndef DIR_H
#define DIR_H
#include "Basic.h"
class Dir{
public:
	Dir(int d = E);
	Dir(const Dir& d);
	static const char * getName(int value) ;
	const char * getName() const;
	int getValue() const;
	static int getByName(const char * s);
	void setValue(int d);
	Dir operator++(int dummy);
	Dir & operator++();	
	void operator+=(int d);
	Dir operator--(int dummy);
	Dir & operator--();	
	bool operator<(Dir d);
	bool operator<=(Dir d);
	bool operator==(Dir d);
	Dir operator+(Dir d);
	// Diferencia angular
	int operator-(Dir d);
	// Direccion inversa
	Dir operator-();
	int shortWay(int angle);
	bool isCurve(Dir d);
	bool isStraight(Dir d);
	char getAspectChar()const;
private:
	int value;
	friend ostream& operator<<(ostream &o, Dir d);
	static Logger log;
};
#endif
