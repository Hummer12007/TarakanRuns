OBJECTS=Team.o Stadium.o Timetable.o
CPPFLAGS=-std=c++11 -Wall -Wextra
VPATH=src:lib:test


all: main

main: $(OBJECTS)

test: $(OBJECTS)

clean: clean_objects clean_symbols

clean_objects:
	find . -type f -name '*.o' -exec rm {} +

clean_symbols:
	find . -type f -name '*.dSYM' -exec rm {} +
