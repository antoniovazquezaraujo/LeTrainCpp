CXXFLAGS= -std=c++0x -ggdb3 -O0 -I. -I$(top_srcdir)/include 
CFLAGS	= -ggdb3 -O0 -I. -I$(top_srcdir)/include 

bin_PROGRAMS = letrain 
check_PROGRAMS = test

TESTS = $(check_PROGRAMS)


ALL_SRCS =                  \
Aspect.cpp                  \
Commander.cpp               \
CommanderView.cpp           \
CrossAspect.cpp             \
CrossRail.cpp               \
CurveAspect.cpp             \
CurveRail.cpp               \
Dir.cpp                     \
DirEnv.cpp                  \
Event.cpp                   \
EventProgram.cpp            \
Filter.cpp                  \
FinderAspect.cpp            \
Finder.cpp                  \
ForkAspect.cpp              \
ForkDirSelector.cpp         \
ForkRail.cpp                \
Game.cpp                    \
Link.cpp                    \
LocomotiveAspect.cpp        \
Locomotive.cpp              \
Message.cpp                 \
Parser.cpp                  \
Point.cpp                   \
ProgramEditor.cpp           \
Console.cpp                 \
Rail.cpp                    \
RailEnv.cpp                 \
RailMap.cpp                 \
RailPen.cpp                 \
RailVehicle.cpp             \
Range.cpp                   \
Semaphore.cpp               \
Sensor.cpp                  \
Sim.cpp                     \
SimView.cpp                 \
StraightAspect.cpp          \
StraightRail.cpp            \
TerminalWindow.cpp          \
Train.cpp                   \
TrainIterator.cpp           \
Vehicle.cpp                 \
WagonAspect.cpp             \
Wagon.cpp                   \
Window.cpp                   

letrain_SOURCES= $(ALL_SRCS) Letrain.cpp
letrain_LDFLAGS = -lncurses -lpanel -llog4cplus 
test_SOURCES = $(ALL_SRCS) Test.cpp
test_LDFLAGS = -lncurses -lpanel -lcpptest -llog4cplus

EXTRA_DIST = log4cplus.properties 
AUTOMAKE_OPTIONS = foreign

