#ifndef ITERATOR_H
#define ITERATOR_H

class Iterator;
#include "node.h"
#include "list.h"

class Iterator{
public:
	Iterator(List * list);
	~Iterator();
	int    hasNext();
	int    hasPrev();
	void * next();
	void * prev();
	void   reset();
	void   toEnd();
private:
	Node * actualNode;
	List * list;
};
#endif
