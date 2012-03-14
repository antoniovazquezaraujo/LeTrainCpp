#ifndef SELECTOR_H
#define SELECTOR_H
#include<iostream>
#include<cassert>
#include<exception>
#include<deque>
#include<vector>
using namespace std;

template <typename TContainer>
class Selector{
public:
	Selector(TContainer * container)
		: container(container),
		selected(0),
		active(false){
	}

	void selectNext(){
		if(!active && container->size() >0){
			active = true;
			selected = 0;
		}else{
			if(selected < container->size()-1){
				selected++;
			}
		}
	}
	void selectPrev(){
		if(!active && container->size() >0){
			active = true;
			selected = 0;
		}else{ 
			if(selected > 0){
				selected--;
			}
		}
	}
	bool isSelected(){
		return (active && container->size() > 0);
	}
	bool atFirst(){
		return active && container->size() > 0 && (selected == 0);
	}
	bool atLast(){
		return active && container->size > 0 && (selected == container->size() -1);
	}
	typename TContainer::iterator getSelected(){
		return container->begin()+selected;
	}
private:
	TContainer * container;
	int selected;
	bool active;
};
#endif
