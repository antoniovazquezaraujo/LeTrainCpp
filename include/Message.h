#ifndef MESSAGE_H
#define MESSAGE_H
#include "Basic.h"
class Message{
public:
	enum {
		FORK_DIR,
		SEMAPHORE,
		TRAIN_SPEED,
		TRAIN_AT_SENSOR_SPEED,
		EVENT_TRAIN_SPEED
	};
	Message(int command, int target, int value);
	int getCommand();
	void setCommand(int command);
	int getTarget();
	void setTarget(int);
	int getValue();
	void setValue(int);
private:
	int command;
	int target;
	int value;
};
#endif
