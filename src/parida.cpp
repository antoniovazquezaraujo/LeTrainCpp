#include <iostream>
using namespace std;
int main(){
	bool continuar = true;
	int num=0;
	do{
		try{
			cin >> num;
			if(num == 7) throw "siete";
			if(num == 8) throw "ocho";
			cout << "Acertaste!";
			continuar=false;
		}catch(int n){
			cout << "error";
		}catch(const char * n){
			cout << "ese no es";
		}

	}while(continuar);
}
