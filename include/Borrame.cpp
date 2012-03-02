#ifndef SELECTOR_H
#define SELECTOR_H
#include<iostream>
#include<cassert>
#include<exception>
#include<vector>
using namespace std;

template <typename TContainer>
class Selector{
public:
	Selector(TContainer * container)
		: container(container),
		selected(-1){
	}

	void selectNext(){
		if(selected < container->size()){
			selected++;
		}
	}
	void selectPrev(){
		if(selected > 0){
			selected--;
		}
	}
	bool isSelected(){
		return (selected != -1);
	}
	bool atFirst(){
		return selected == 0;
	}
	bool atLast(){
		return selected == container->size() -1;
	}
	typename TContainer::iterator getSelected(){
		return container->begin()+selected;
	}
private:
	TContainer * container;
	int selected;
};
int main(){
	vector<int>v;
	Selector<vector<int>> s(&v);
	cout <<"Seleccionado: " << s.isSelected() <<endl;
	s.selectNext();
	cout <<"Seleccionado: " << s.isSelected() <<endl;
	s.selectPrev();
	cout <<"Seleccionado: " << s.isSelected() <<endl;

	v.push_back(23);
	cout <<"Seleccionado: " << s.isSelected() <<endl;

	s.selectNext();
	cout << "despues";
	cout << endl<< *s.getSelected() << endl; 
	cout <<"Seleccionado: " << s.isSelected() <<endl;
	cout << endl<< *(s.getSelected()) << endl; 
	s.selectPrev();
	cout <<"Seleccionado: " << s.isSelected() <<endl;
	cout << endl<< *(s.getSelected()) << endl; 
}
#endif
