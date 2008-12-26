#include "Basic.h"
#include "EventProgram.h"
#include "Event.h"
#include "Window.h"
#include "Message.h"
#include "Dir.h"
#include "Filter.h"


Filter * EventProgram::getFilter(){
	return filter;
}
void EventProgram::setFilter(Filter * filter){
	this->filter = filter;
}
bool EventProgram::accept(Event * event){
	return filter->accept(event);
}
EventProgram::CMessages * EventProgram::getMessages(){
	return &messages;
}
void EventProgram::addMessage(Message * message){
	messages.push_back(message);
}

/*
vector<Message*>::iterator EventProgram::begin(){
	return messages.begin();
}
vector<Message*>::iterator EventProgram::end(){
	return messages.end();
}
*/
