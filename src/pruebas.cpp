#include <iostream>
using namespace std;
class X{
public:
	X(int size)
	:size(size), data(new char[size]){
		
	}
	int size;
	char * data;
	~X(){
		for(int n=0; n<size; n++){
			data[n] = '/';
		}
		delete [] data;
	}
	void llenar(){
		for(int n=0; n<size; n++){
			data[n] = 'X';
		}
	}

};
X create();
int main(){
	X x = create();
	X y = create();
	X z = create();
	x.llenar();
	y.llenar();
	z.llenar();
}
X create(){
	X x(100);
	return x;
}
