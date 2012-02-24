#ifndef RANGE_H
#define RANGE_H
#include "Basic.h" 
#include <vector> 
#include <algorithm> 
#include <iostream> 
using namespace std;
class Range{
public:
	Range();
	Range(const Range & r);
	bool accept(int n);
	void acceptAll(bool );
	void addNumber(int n);
	void addGtCondition(int n);
	void addLtCondition(int n);
	void addPair(int n, int m);
	string str();
private:
	vector<pair<int, int> > ranges;
	vector<int>  gtConditions;
	vector<int>  ltConditions;
	vector<int>  numbers;
	bool acceptingAll;
	friend ostream & operator << (ostream & out, Range & r);
	static Logger log;
};
#endif
