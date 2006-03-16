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
	Listdata data;
	Linkedlist next;
};
struct list{
	Linkedlist start;
};
struct iterator{
	Linkedlist actualNode;
};

static Linkedlist l_l_add(Linkedlist list, Listdata data);
static Linkedlist l_l_del(Linkedlist list, Listdata data);
static int l_l_contains(Linkedlist list, Listdata data);

List l_new(){
	List ret = malloc(sizeof(struct list));
	ret->start = NULL;
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
	list->start = l_l_add(list->start, data);
}
void l_del(List list, Listdata data){
	list->start = l_l_del(list->start, data);
}
int l_contains(List list, Listdata data){
	return l_l_contains(list->start, data);
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
	ret->actualNode= NULL;
	i_reset(ret, list);
	return ret;
}
void i_free(Iterator i){
	assert(i != NULL);
	free(i);
}
int i_hasnext(Iterator i){
	return (i->actualNode != NULL);
}
Listdata i_next(Iterator i){
	Linkedlist actual = i->actualNode;
	i->actualNode = i->actualNode->next;
	return actual->data;
}
void i_reset(Iterator i, List list){
	i->actualNode = list->start;
}
static Linkedlist l_l_add(Linkedlist list, Listdata data){
	Linkedlist node = malloc(sizeof(struct listNode));
	node->data = data;
	node->next = list;
	return node;
}
static Linkedlist l_l_del(Linkedlist list, Listdata data){
	Linkedlist start = list;
	if(list->data == data){
		start = list->next;
		free(list);
	}else{
		while(list != NULL){
			if(list->next != NULL){
				if(list->next->data == data){
					Linkedlist deleted = list->next;
					list->next = list->next->next;
					free(deleted);
					break;
				}else{
					list = list->next;
				}
			}
		}
	}
	return start;
}
static int l_l_contains(Linkedlist list, Listdata data){
	while(list != NULL){
		if(list->data == data){
			return 1;
		}else{
			list = list->next;
		}
	}
	return 0;
}
