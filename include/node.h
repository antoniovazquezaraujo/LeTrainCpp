#ifndef NODE_H
#define NODE_H

class Node {
public:
	Node();
	~Node();
	Node * prev;
	void * data;
	Node * next;
};
#endif
