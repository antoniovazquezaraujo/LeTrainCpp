#include "Range.h"
#include <vector> 
#include <sstream> 
#include <algorithm> 
using namespace std;
MAKE_LOGGER(Range);
Range::Range()
	:acceptingAll(false){

}
Range::Range(const Range & r){
	LOG_DEBUG(log," OJO, COPIANDO UN RANGO!!!!!!!!!!!!!!");
}
bool Range::accept(int n){
	LOG_DEBUG(log,"Entramos en Range::accept con " << n); 
	if(acceptingAll){
		LOG_DEBUG(log," Rango acepta TODO ");
		return true;
	}
		
	LOG_DEBUG(log," El rango tiene " << numbers.size() << " numeros " );
	vector<int>::iterator i;
	for(i=numbers.begin(); i != numbers.end(); i++){
		LOG_DEBUG(log," probando numero " << n << " contra " << (*i));
		if(n == (*i)){
			LOG_DEBUG(log," Rango acepta numero " << n);
			return true;
		}
	}
	for(i=gtConditions.begin(); i != gtConditions.end(); i++){
		if(n > (*i)){
			LOG_DEBUG(log," Rango acepta numero mayor que "<<  (*i) << ":" << n);
			return true;
		}
	}
	for(i=ltConditions.begin(); i != ltConditions.end(); i++){
		if(n < (*i)){
			LOG_DEBUG(log," Rango acepta numero menor que "<<  (*i) << ":" << n);
			return true;
		}
	}
	vector<pair<int, int> >::iterator ir;
	for(ir=ranges.begin(); ir != ranges.end(); ir++){
		if(n >= (*ir).first && n <= (*ir).second){
			LOG_DEBUG(log," Rango acepta numero entre " << (*ir).first << " y "<<  (*ir).second << ":" <<  n);
			return true;
		}
	}
	LOG_DEBUG(log," Rango NO ACEPTA el numero " << n);
	return false;
}
void Range::acceptAll(bool acceptAll){
	this->acceptingAll = acceptAll;
}
void Range::addNumber(int n){
	LOG_DEBUG(log," Rango agregando numero " << n);
	numbers.push_back(n);
	LOG_DEBUG(log," Ahora el rango tiene " << numbers.size() << " numeros");
	acceptingAll=false;
}

void Range::addGtCondition(int n){
	gtConditions.push_back(n);
	acceptingAll=false;
}

void Range::addLtCondition(int n){
	ltConditions.push_back(n);
	acceptingAll=false;
}

void Range::addPair(int n, int m){
	ranges.push_back(make_pair(n,m));
	acceptingAll=false;
}
string Range::str(){
	if(acceptingAll){
		return "*";
	}
	stringstream s("");
	int n = 0;
	for(vector<int>::iterator i = numbers.begin();i!= numbers.end();i++){
		if(n++ > 0) s << ',';
		s << *i ;
	}
	for(vector<int>::iterator i = gtConditions.begin();i!= gtConditions.end();i++){
		if(n++ > 0) s << ',';
		s << '>' << *i ;
	}
	for(vector<int>::iterator i = ltConditions.begin();i!= ltConditions.end();i++){
		if(n++ > 0) s << ',';
		s << '<' << *i ;
	}
	for(vector<pair<int, int> >::iterator i = ranges.begin();i!= ranges.end();i++){
		if(n++ > 0) s << ',';
		s << (*i).first  << '-' << (*i).second ;
	}
	return s.str();
}
ostream & operator << (ostream & out, Range & r){
	if(r.acceptingAll){
		out <<endl << "*" <<endl;
		return out;
	}
	out <<endl;
	out <<"Numeros: "<<endl;
	for(vector<int>::iterator i = r.numbers.begin();i!= r.numbers.end();i++){
		out << *i << endl;
	}
	out <<"Mayores que...: "<<endl;
	for(vector<int>::iterator i = r.gtConditions.begin();i!= r.gtConditions.end();i++){
		out << *i << endl;
	}
	out <<"Menores que...: "<<endl;
	for(vector<int>::iterator i = r.ltConditions.begin();i!= r.ltConditions.end();i++){
		out << *i << endl;
	}
	out <<"Rangos: "<<endl;
	for(vector<pair<int, int> >::iterator i = r.ranges.begin();i!= r.ranges.end();i++){
		out << (*i).first << " -> " << (*i).second<< endl;
	}
	return out;
}
