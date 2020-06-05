.DEFAULT_GOAL := all

game : win.c game.c main.c
	# c++ win.c -DDEBUG_MAIN -g -lpthread -lncurses 
	c++ main.c win.c game.c -g -Wall -O0 -lpthread -lncurses -I/usr/include/ncurses -o clitank

install : clitank
	mkdir -p /usr/local/clitank
	cp ./clitank /usr/local/clitank/clitank

.PHONY : all
all : game

clean:
	@rm -f a.out
	@rm -f dot
	@rm -f key
	@rm -f *.o
	@rm -f clitank