OBJECTS=stringutils.o logging.o MultiCallHandler.o \
        Client.o Server.o ClientSocket.o \
        RandomStrategy.o Tarakan.o Team.o Timetable.o Race.o \
            RaceResult.o Stadium.o Management.o \
        multi_call_main.o
CXXFLAGS=-std=c++11 -g -Wall -Wextra \
          -Isrc -Isrc/include
LDFLAGS=-Llib
LDLIBS=-lnetlink -pthread
VPATH=src/util:src/network:src/trk


all: tarakans team management docs


tarakans: lib/libnetlink.a $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

lib/libnetlink.a: lib/Makefile
	$(MAKE) -C lib

team: tarakans
	ln -s tarakans team

management: tarakans
	ln -s tarakans management


.PHONY: docs
docs: docs/Makefile
	$(MAKE) -C docs


clean: clean-obj clean-exec clean-symlinks

clean-obj:
	find . -maxdepth 1 -type f -name '*.o' -exec rm -r {} +

clean-exec:
	find . -maxdepth 1 -type f -executable -exec rm -r {} +

clean-symlinks:
	find . -maxdepth 1 -type l -exec rm -r {} +
