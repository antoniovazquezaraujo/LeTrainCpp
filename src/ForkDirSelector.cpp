#include "Basic.h"
#include "ForkDirSelector.h"
#include "DirEnv.h"

ForkDirSelector::ForkDirSelector(DirEnv * env)
	: env(env){
	for (int n(0); n<3;n++){
		outs[n] = Dir::NO_DIR;
	}
	selectedOut=0;
	numOuts = 0;
//cout << "el env es: " <<endl;
//cout << *env<<endl;

	for (int n(Dir::E); n<Dir::NUM_DIRS;n++){
		Dir dir(n);
		Dir found = env->getPath(dir);
		if(found.getValue() != Dir::NO_DIR){
//			cout << "Buscando la salida por: " << dir.getValue() << "= " << found.getValue()<<endl;
			if(env->canBeAForkInput(dir)){
				in = dir;
//				cout << "Entrada: " << in<< endl;
			}else{
				//cout << "Out: "<< numOuts << "=" << n<< endl;
				selectedOut=numOuts;
				//outs[numOuts++] = found;
				outs[numOuts++] = dir;
			}
		}
	}
	assert(numOuts <= 3);
//	cout << "Salidas: " << endl;
//	for(int n(0); n< 3; n++){
//		cout << n <<" : " << outs[n] << endl;
//	}
}
void ForkDirSelector::selectDir(Dir dir){
	for(int n(0); n< numOuts; n++){
		if(outs[n] == dir){
			selectedOut = n;
//			cout << "seleccionada salida : " << n << " valor: " << outs[n]<<endl;
			break;
		}
	}	
}
Dir ForkDirSelector::getSelectedDir(){
	return outs[selectedOut];
}
void ForkDirSelector::setNextDir(){
	selectedOut++;
	selectedOut%=numOuts;
}
void ForkDirSelector::setPrevDir(){
	selectedOut--;
	if(selectedOut < 0){
		selectedOut=numOuts-1;
	}
}
Dir ForkDirSelector::getPath(Dir dir){
	if(dir == in){
		return outs[selectedOut];
	}else{
		return in;
	}
}
ostream & operator << (ostream & o, ForkDirSelector & selector){
	for(int n = 0; n< selector.numOuts;n++){
		o << "[" << (selector.outs[n]).getValue() << "]";
	}
	o <<  (selector.in).getValue() << "->" << "(" << (selector.outs[selector.selectedOut]).getValue() << ")"; 
	return o;
}
