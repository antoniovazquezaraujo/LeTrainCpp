AM_CXXFLAGS	= -ggdb -L$(top_srcdir)/lib -I$(top_srcdir)/include/UnitTest++ -I$(top_srcdir)/include -W -Wall 
bin_PROGRAMS = letrain carriles testing

ALL_SRCS =\
	list.cpp\
	point.cpp\
	dir.cpp\
	alert.cpp\
	iterator.cpp\
	sim.cpp\
	charmap.cpp\
	railmap.cpp\
	oldRailmap.cpp\
	finder.cpp\
	rail.cpp\
	cross.cpp\
	commander.cpp\
	vehicle.cpp\
	graphics.cpp\
	node.cpp\
	trainiterator.cpp\
	logging.cpp\
	persist.cpp

letrain_SOURCES=\
	$(ALL_SRCS)\
	letrain.cpp

letrain_LDFLAGS = -lncurses

testing_SOURCES =  $(ALL_SRCS) Test.cpp \
	TestNewRailMap.cpp

testing_LDADD = $(top_srcdir)/lib/libUnitTest++.a 
testing_LDFLAGS = -lncurses

carriles_SOURCES = carriles.cpp
carriles_LDFLAGS = -lncurses

EXTRA_DIST = $(ALL_SRCS) letrain.cpp carriles.cpp 

AUTOMAKE_OPTIONS = foreign
