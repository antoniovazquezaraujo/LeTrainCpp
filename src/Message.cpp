#include "Message.h"
Message::Message(int command, int target, int value)
	:command(command), target(target), value(value){

	}
int Message::getCommand(){
	return command;
}
void Message::setCommand(int command){
	this->command = command;
}
int Message::getTarget(){
	return target;
}
void Message::setTarget(int target){
	this->target = target;
}
int Message::getValue(){
	return value;
}
void Message::setValue(int value){
	this->value = value;
}
