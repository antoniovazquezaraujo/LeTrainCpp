AM_CXXFLAGS	= -O2 -std=c++98 -ggdb -I. -I$(top_srcdir)/include -W -Wall 

bin_PROGRAMS = letrain 

check_PROGRAMS = test

TESTS = $(check_PROGRAMS)

ALL_SRCS =                  \
	CrossAspect.cpp         \
	CrossRail.cpp           \
	CurveAspect.cpp         \
	CurveRail.cpp           \
	Dir.cpp                 \
	DirEnv.cpp              \
	ForkDirSelector.cpp     \
	ForkRail.cpp            \
	Rail.cpp                \
	StraightAspect.cpp      \
	StraightRail.cpp        \
	Aspect.cpp              \
	ForkAspect.cpp          \
	Point.cpp               \
	Vehicle.cpp             \
	RailVehicle.cpp         \
	RailMap.cpp             \
	RailPen.cpp             \
	Window.cpp              \
	TerminalWindow.cpp      \
	Commander.cpp           \
	Sim.cpp                 \
	SimView.cpp             \
	TrainIterator.cpp       \
	CommanderView.cpp       \
	Finder.cpp              \
	FinderAspect.cpp        \
	Train.cpp               \
	Locomotive.cpp          \
	RailEnv.cpp             \
	Game.cpp                \
	Wagon.cpp               \
	WagonAspect.cpp         \
	LocomotiveAspect.cpp    \
	Event.cpp               \
	EventProgram.cpp        \
	Sensor.cpp              \
	Message.cpp             \
	Semaphore.cpp           \
	Range.cpp               \
	Filter.cpp              \
	ProgramEditor.cpp       \
	Link.cpp                \
	Parser.cpp

letrain_SOURCES= $(ALL_SRCS) Letrain.cpp
letrain_LDFLAGS = -lncurses -lpanel -lcpptest -llog4cplus 
test_SOURCES = $(ALL_SRCS) Test.cpp
test_LDFLAGS = -lncurses -lpanel -lcpptest -llog4cplus

EXTRA_DIST = log4cplus.properties programa.ltr
AUTOMAKE_OPTIONS = foreign

