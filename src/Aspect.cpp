#include "Basic.h"
#include "Aspect.h"
#include "Dir.h"
char Aspect::getAspectChar(){
	return '?';
}
void Aspect::updateAspect(Dir d){
	//para que no proteste por no usarlo!
	d++;
}
