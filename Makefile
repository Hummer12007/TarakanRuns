OBJECTS=Tarakan.o Team.o Track.o Stadium.o Management.o Broker.o Timetable.o
CFLAGS=-std=c++11 -Wall -Wextra
VPATH=src

all: main

main: $(OBJECTS)

split: str_list.o

clean_objects:
	rm -f *.o

clean_symbols:
	rm -f -r *.dSYM

clean: clean_objects clean_symbols

