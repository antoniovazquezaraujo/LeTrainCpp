#include <iostream> 
#include "cpptest.h"
#include "RailTestSuite.h"
class LetrainTest{
public:
	static bool test(){
		Test::Suite ts;
		ts.add(auto_ptr<Test::Suite>(new RailTestSuite));
		Test::TextOutput output(Test::TextOutput::Verbose);
		bool retorno = ts.run(output, false) ;
		std::cout << "retornando " << retorno<< std::endl;
		return retorno;
	}
};
