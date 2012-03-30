#include <iostream>
using namespace std;
void f(int (&a)[2][3]){
	cout << sizeof(a) << ", " << sizeof(a[0]) << ", " << sizeof(a[0][0])<<endl;
}
void g(int (a)[][3]){
	cout << sizeof(a) << ", " << sizeof(a[0]) << ", " << sizeof(a[0][0])<<endl;
}
void g(int (a)[][4]){
	cout << sizeof(a) << ", " << sizeof(a[0]) << ", " << sizeof(a[0][0])<<endl;
}
int main(){
	int a[2][3];
	int b[12][3];
	int c[12][4];
	f(a);
	f(a);
	g(b);
	g(c);
}

