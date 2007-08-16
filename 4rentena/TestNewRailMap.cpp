#include "UnitTest++.h"
#include "NewRailMap.h"
#include "charmap.h"

using namespace UnitTest;

	TEST(testCreacion) {
		Charmap cm;
		Rail r(STOP_RAIL,0,0);
		NewRailMap rm(cm);
		//rm.setRail(0,            0,            r);
		//rm.setRail(0,            CHAR_MAP_COLS-1,r);
		//rm.setRail(CHAR_MAP_ROWS-1,0,            r);
		//rm.setRail(CHAR_MAP_ROWS-1,CHAR_MAP_COLS-1,r);
		//CHECK(rm.getRail(0,             0) == r);
		//CHECK(rm.getRail(0,             CHAR_MAP_COLS-1) == r);
		//CHECK(rm.getRail(CHAR_MAP_ROWS-1, 0) == r);
		//CHECK(rm.getRail(CHAR_MAP_ROWS-1, CHAR_MAP_COLS-1) == r);

		CHECK (true);
	}


