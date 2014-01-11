c++ w.c -DDEBUG_MAIN -g -lpthread -lncurses 
c++ g.c c.c  -DDEBUG_MAIN -g -lncurses -o key
c++ h.c c.c w.c g.c  -g -Wall -O0 -lpthread -lncursesw -I/usr/include/ncurses -o dot
