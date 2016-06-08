OBJECTS=core.o smart_buffer.o socket.o socket_group.o util.o
CXXFLAGS=-g -I../src/include
VPATH=netlink/src


all: libnetlink.a

libnetlink.a: $(OBJECTS)
	$(AR) rcs $@ $^


clean: clean-obj clean-lib.a

clean-obj:
	find . -maxdepth 1 -type f -name '*.o' -exec rm -r {} +

clean-lib.a:
	find . -maxdepth 1 -type f -name '*.a' -exec rm -r {} +
