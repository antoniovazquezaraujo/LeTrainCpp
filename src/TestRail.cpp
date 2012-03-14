#include "UnitTest++.h"
#include "rail.h"

using namespace UnitTest;

	TEST(TestRail1) {
		Rail r(STOP_RAIL, 0,0);
		r.setDir(0, S);
		CHECK (r.getDir(0) == S);
	}


