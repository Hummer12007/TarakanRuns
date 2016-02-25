OBJECTS=
CFLAGS=-std=c++11 -Wall -Wextra
VPATH=src

all: main

main: $(OBJECTS)

split: str_list.o

format:
	clang-format -i /[A-Z]*

clean_objects:
	rm -f *.o

clean_symbols:
	rm -f -r *.dSYM

clean: clean_objects clean_symbols

