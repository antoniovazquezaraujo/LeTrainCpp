#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include "Basic.h"
#include "Point.h"
class Semaphore{
public:
	enum{CLOSED, OPEN};
	Semaphore(int id, bool open=true);
	int getId();
	void setId(int id);
	bool isOpen();
	void close();
	void open();
	void paint(Window * g);
	void erase(Window * g);
	Point getPos();
	void setRow(int row);
	void setCol(int col);
private:
	Point pos;
	int id;
	bool openValue;
};
#endif
