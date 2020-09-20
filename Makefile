.DEFAULT_GOAL := all

CXX = @c++
CFLAGS = -Wall -g
LIBLINUX = -I/usr/include/ncurses
LIBMAC = -I/usr/local/Cellar/ncurses/6.2/lib

main: w.cpp g.cpp c.cpp h.cpp
	$(CXX) $(CFLAGS) w.cpp -DDEBUG_MAIN -lpthread -lncurses 
	$(CXX) $(CFLAGS) g.cpp c.cpp -DDEBUG_MAIN -lncurses -o key.out
	$(CXX) $(CFLAGS) h.cpp c.cpp w.cpp g.cpp -O0 -lpthread -lncursesw $(LIBLINUX) -o dot.out

.PHONY : all
all : main

clean:
	@rm -f *.out
	@rm -f *.o
	@rm -f *.a
	@rm -rf *.out.*