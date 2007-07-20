/*
  iterator.cpp
  container iterator
  -------------------------------------
*/
#include "letrain.h"
Iterator::Iterator (List * list){
	this->list = list;
	reset();
}
Iterator::~Iterator(){
}
int Iterator::hasNext(){
	return (actualNode->next != list->end);
}
int Iterator::hasPrev(){
	return (actualNode->prev != list->start);
}
void * Iterator::next(){
	actualNode = actualNode->next;
	return actualNode->data;
}
void * Iterator::prev(){
	actualNode = actualNode->prev;
	return actualNode->data;
}
void Iterator::reset(){
	actualNode = list->start;
}
void Iterator::toEnd(){
	actualNode = list->end;
}
