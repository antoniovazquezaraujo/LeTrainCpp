#ifndef EVENTPROGRAM_H
#define EVENTPROGRAM_H
#include "Basic.h"
#include "Event.h"
#include "Filter.h"
class EventProgram{
public:
	void setFilter(Filter * filter);
	Filter *getFilter();
	typedef vector<Message*> CMessages;
	CMessages * getMessages();
	void addMessage(Message * message);
	bool accept(Event * event);
	//vector<Message*>::iterator begin();
	//vector<Message*>::iterator end();
private:
	Filter * filter;
	CMessages messages;
	static Logger log;
};
#endif
