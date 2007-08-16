#ifndef LIST_H
#define LIST_H

class List;
#include "iterator.h"
#include "node.h"

class List{
	friend class Iterator;
public:
	List(); 
	~List(); 
	void       add(void * data);
	void       insert(void * data);
	void       append(void * data);
	void       del(void * data);
	int        contains(void * data);
	void *     get(int index);
	Iterator * iterator();
private:
	Node * start;
	Node * end;
};

#endif
