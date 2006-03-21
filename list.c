/*
  list.c
  Object container
  -------------------------------------
*/
#include <stdlib.h>
#include <assert.h>
#include "list.h"
typedef struct listNode * Linkedlist;
struct listNode {
	Linkedlist prev;
	Listdata data;
	Linkedlist next;
};
struct list{
	Linkedlist start;
	Linkedlist end;
};
struct iterator{
	Linkedlist actualNode;
	List list;
};

List l_new(){
	List ret = malloc(sizeof(struct list));
	Linkedlist start= malloc(sizeof(struct listNode));
	start->data = NULL;
	Linkedlist end  = malloc(sizeof(struct listNode));
	end->data = NULL;
	ret->start = start;
	ret->end = end;
	start->next = end;
	start->prev = NULL;
	end->next = NULL;
	end->prev = start;
	return ret;
}
void l_free(List list){
	Linkedlist next= list->start;
	Linkedlist actual = next; 
	while(next!= NULL){
		next = next->next;
		free(actual);
		actual = next;
	}
	free(list);
}
void l_add(List list, Listdata data){
	l_insert(list, data);
}
void l_insert(List l, Listdata data){
	Linkedlist node = malloc(sizeof(struct listNode));
	node->data = data;
	node->next = l->start->next;
	node->prev = l->start;
	l->start->next = node;
	node->next->prev = node;
}
void l_append(List l, Listdata data){
	Linkedlist node = malloc(sizeof(struct listNode));
	node->data = data;
	node->prev= l->end->prev;
	node->next= l->end;
	l->end->prev= node;
	node->prev->next= node;
}
void l_del(List list, Listdata data){
	Linkedlist node;
	Iterator i = i_new(list);
	while(i_hasnext(i)){
		node = (Linkedlist)i_next(i);
		if(node->data == data){
			node->prev->next = node->next;
			node->next->prev = node->prev;
			free(node);
			i_free(i);
			return;
		}
	}
	i_free(i);
}
int l_contains(List list, Listdata data){
	Linkedlist node;
	Iterator i = i_new(list);
	while(i_hasnext(i)){
		node = (Linkedlist)i_next(i);
		if(node->data == data){
			i_free(i);
			return 1;
		}
	}
	i_free(i);
	return 0;
}
Listdata l_get(List list, int index){
	int count = 0;
	Listdata ret = NULL;
	Iterator i = i_new(list);
	while(i_hasnext(i)){
		if (count == index){
			ret = i_next(i);
			break;
		}else{
			count++;
			i_next(i);
		}
	}
	assert(i != NULL);
	i_free(i);
	return ret;
}
Iterator i_new(List list){
	Iterator ret = malloc(sizeof(struct iterator));
	ret->list = list;
	i_reset(ret);
	return ret;
}
void i_free(Iterator i){
	assert(i != NULL);
	free(i);
}
int i_hasnext(Iterator i){
	return (i->actualNode->next != i->list->end);
}
int i_hasprev(Iterator i){
	return (i->actualNode->prev != i->list->start);
}
Listdata i_next(Iterator i){
	i->actualNode = i->actualNode->next;
	return i->actualNode->data;
}
Listdata i_prev(Iterator i){
	i->actualNode = i->actualNode->prev;
	return i->actualNode->data;
}
void i_reset(Iterator i){
	i->actualNode = i->list->start;
}
void i_toend(Iterator i){
	i->actualNode = i->list->end;
}
