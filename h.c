#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <term.h>
#include <curses.h>
#include <sys/ioctl.h>
#include <map>
#include <string>

/*
by pyw 2014-1-9
gcc h.c -g -O0 -lncurses -I/usr/include/ncurses -o h
*/

static struct termios initial_settings, new_settings;
static int peek_character = -1;

typedef struct _Locate
{
    int x;
    int y;
    int w;
    int h;
} Locate;

typedef struct _Win
{
    Locate locate;
    char name[64];
    char title[64];
    WINDOW *win;
} Win;

typedef std::map<std::string, Win> Windows;
Windows frames; // all frames
Windows windows; // all content windows



void init_keyboard()
{
    tcgetattr(0,&initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_lflag &= ~ISIG;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
}
void close_keyboard()
{
    tcsetattr(0, TCSANOW, &initial_settings);
}

int kbhit()
{
    char ch;
    int nread;
    if(peek_character != -1) {
        return 1;
    }
    new_settings.c_cc[VMIN]=0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0,&ch,1);
    new_settings.c_cc[VMIN]=1;
    tcsetattr(0, TCSANOW, &new_settings);
    if(nread == 1) {
        peek_character = ch;
        return 1;
    }
    return 0;
}

int readch()
{
    char ch;
    if(peek_character != -1) {
        ch = peek_character;
        peek_character = -1;
        return ch;
    }
    read(0,&ch,1);
    return ch;
}

int form_back(WINDOW *win)
{
    WINDOW *dialog;
    int y = 10;
    int x = 10;
    int height = 20;
    int width = 20;

    wattrset(win, 0);
	wmove(win, y, x);
    waddch(win, A_NORMAL| ACS_ULCORNER);
	waddstr(win, "hello");
    wrefresh (win);

    return 0;
}

int resize_allwin()
{
    Windows::iterator iter;
    struct winsize size;
    ioctl(STDIN_FILENO,TIOCGWINSZ,&size);
    printf("%d\n",size.ws_col);
    printf("%d\n",size.ws_row);


    for (iter=windows.begin(); iter!=windows.end(); iter++) {
        if (iter->first.compare("L1") == 0) {
            iter->second.locate.w = size.ws_col * 0.8;
        } else {
            iter->second.locate.w = size.ws_col * 0.2;
        }
        iter->second.locate.h = size.ws_row - 1;
        box(iter->second.win, '|', '-');
        wrefresh(iter->second.win);
    }
}

int init_allwin()
{
    int ret =0;
    int i = 0;
    Win win[4] = {
            // x,y,width,height,name,title,WINDOW*
            {{0, 0, 60, 60}, "L1", NULL, NULL}, 
            {{100, 0, 20, 20}, "R1", NULL, NULL}, 
            {{100, 10, 20, 20}, "R2", NULL, NULL}, 
            {{100, 15, 20, 20}, "R3", NULL, NULL}, 
            };


    for (i=0; i<4; i++) {
        win[i].win = newwin(win[i].locate.y, win[i].locate.x, 0, 0);
        if (NULL == win) {
            printf("Init failed.\n");
            ret = -1;
            break;
        }
        scrollok(win[i].win, TRUE);
        box(win[i].win, '|', '-');
        wrefresh(win[i].win);
        windows.insert(Windows::value_type(win[i].name, win[i]));
    }
 
    return ret;
}

int main(int argc, char *argv[])
{
    int ret = 0;
    int height = 20;
    int width = 20;
    int y = 0;
    int x = 0;
    int key = 0;
    int ch = 0;
    WINDOW *win = NULL;

    init_keyboard();
    initscr();

    init_allwin();
    resize_allwin();

    /*
    // form_back(win);

	key = wgetch(win);
    wmove(win,2,3);
	key = wgetch(win);
	waddstr(win, "hello");
	key = wgetch(win);
    */
aaa:
    while(ch != 'q') {
        usleep(500);
        if(kbhit()) {
            ch = readch();
            wprintw(win, ".");
            wrefresh(win);
        }
        wprintw(win, ".");
        wrefresh(win);
    }

quit:
    close_keyboard();

    endwin();

    for (int i = 30; i<129;i++) {
        printf("%d %c\t", i, i);
    }
    printf("\n");


    system("stty sane");

    return ret;
}
