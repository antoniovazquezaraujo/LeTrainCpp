#ifndef RAIL_TEST_SUITE
#define RAIL_TEST_SUITE

#include <cstdlib>
#include <cstring>
#include <iostream>
#include "cpptest.h"
#include "rail.h"
#include "dir.h"
using namespace std;

class RailTestSuite : public Test::Suite {
public:
	RailTestSuite() {
		TEST_ADD(RailTestSuite::test_type)
		TEST_ADD(RailTestSuite::test_dir)
		runTest();
	}
	void runTest(){
		Test::Output* output = new Test::CompilerOutput;
		try {
			run(*output, true);
		} catch (...) {
			cout << "unexpected exception encountered\n";
		}
	}
private:
	void test_type() {
		TEST_ASSERT(r->type == STOP_RAIL);
		TEST_ASSERT(r->type == 43);
	}
	
	void test_dir() {
		r->setDir(0, N);
		TEST_ASSERT(r->getDir(0) == N);
	}
	void setup(){
		r = new Rail(STOP_RAIL, 0,0);
	}
	void tear_down(){
		delete r;
		r = 0;
	}
	Rail * r;
};
#endif
