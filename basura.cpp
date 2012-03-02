#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
template <typename T, typename F>
void for_each(T col, int inc, F fun){
	typename T::iterator i = col.begin();
	while(i != col.end()){
		fun(*i);
		int n = 0;
		while(n< inc && i != col.end()){
			i++; n++;
		}
	}
}
int main(){
	vector<int>v{1,2,3,4,5};
	for_each(v,2, [](int i){
		cout << i;
	});
	vector<int> * p = nullptr;
	if(p) cout << "SI";
	else cout << "NO";
}
