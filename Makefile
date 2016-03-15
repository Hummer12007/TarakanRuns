OBJECTS=Team.o Stadium.o Timetable.o Broker.o
CPPFLAGS=-Ilib
CXXFLAGS=-std=c++11 -Wall -Wextra
VPATH=src

all: main

main: $(OBJECTS)

Test: $(OBJECTS)

UnitTest: $(OBJECTS)

clean: clean_objects clean_symbols

clean_objects:
	find . -type f -name '*.o' -exec rm -rf {} +

clean_symbols:
	find . -type f -name '*.dSYM' -exec rm -rf {} +
