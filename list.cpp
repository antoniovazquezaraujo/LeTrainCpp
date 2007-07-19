/*
  List.cpp
  Object container
  -------------------------------------
*/
#include "letrain.h"
List::List(){
	start = new Node();
	end = new Node();

	start->next = end;
	end->prev = start;

	end->next = end;
	start->prev = start;
}
List::~List(){
	Node * next= start;
	Node * actual = next; 
	while(next!= end){
		next = next->next;
		delete actual;
		actual = next;
	}
	delete next;
}
Iterator *  List::iterator(){
	return new Iterator(this);
}
void List::add(void * data){
	insert(data);
}
void List::insert(void * data){
	Node * node = new Node(); 
	node->data = data;
	node->next = start->next;
	node->prev = start;
	start->next = node;
	node->next->prev = node;
}
void List::append(void * data){
	Node * node = new Node(); 
	node->data = data;
	node->prev= end->prev;
	node->next= end;
	end->prev= node;
	node->prev->next= node;
}
void List::del(void * data){
	Node * node = start;
	while(node != end){
		node = node->next;
		if(node->data == data){
			node->prev->next = node->next;
			node->next->prev = node->prev;
			delete node;
			return;
		}
	}
}
int List::contains(void * data){
	void * next;
	Iterator * i = iterator();
	while(i->hasNext()){
		next= (void *)i->next();
		if(next == data){
			delete i;
			return 1;
		}
	}
	delete i;
	return 0;
}
void * List::get(int index){
	int count = 0;
	void * ret = NULL;
	Iterator * i = iterator(); 
	while(i->hasNext()){
		if (count == index){
			ret = i->next();
			break;
		}else{
			count++;
			i->next();
		}
	}
	assert(i != NULL);
	delete i;
	return ret;
}
