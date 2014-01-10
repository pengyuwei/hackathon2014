c++ w.c -DDEBUG_MAIN -g -lpthread -lncurses
c++ h.c c.c w.c  -g -Wall -O0 -lpthread -lncurses -I/usr/include/ncurses -o dot
