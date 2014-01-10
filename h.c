#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

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

typedef struct _Scale
{
    float x;
    float y;
    float w;
    float h;
} Scale;


typedef struct _Win
{
    Locate locate;
    Scale scale;
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
void cleanup()
{
    endwin();
    system("stty sane");
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
    Win *win = NULL;
    Windows::iterator iter;
    struct winsize size;
    char msg[64] = {0};
    int ret = 0;

    ioctl(STDIN_FILENO,TIOCGWINSZ,&size);

    for (iter=windows.begin(); iter!=windows.end(); iter++) {
        win = &iter->second;
        win->locate.x = size.ws_col * win->scale.x + 1;
        win->locate.y = size.ws_row * win->scale.y + 1;
        win->locate.w = size.ws_col * win->scale.w - 2;
        win->locate.h = size.ws_row * win->scale.h - 2;

        if (iter->first.compare("RCMD") == 0) {
            win->locate.h = 4;
        }

        ret = wresize(win->win, win->locate.h, win->locate.w);
        if (ERR == ret) {
            printf("wresize failed:%s %d,%d\n", win->name, win->locate.x, win->locate.y);
        }
        ret = mvwin(win->win, win->locate.y, win->locate.x);
        if (ERR == ret) {
            cleanup();
            printf("SCREEN=%d x %d\n", COLS, LINES);
            printf("%d\n", win->win);
            printf("mvwin failed:%s %d,%d\n", win->name, win->locate.x, win->locate.y);
            printf("\t %d x %d\n", win->locate.w, win->locate.h);
            exit(0);
        }

        //echo();
        //touchwin(win->win);
        //attron(A_REVERSE);
        box(win->win, '|', '-');
        //mvaddstr(2, 2,win->name);
        mvwaddstr(win->win, 1, 1, win->name);
        sprintf(msg, "%d,%d", win->locate.y, win->locate.x);
        mvwaddstr(win->win, 2, 1, msg);
        //attroff(A_REVERSE);
        //move(1, 1);
        //waddstr(win->win, win->name);
        wrefresh(win->win);
  //      }
    }
}

int init_allwin()
{
    int ret =0;
    int i = 0;
    Win win[4] = {
            // x,y,width,height,name,title,WINDOW*
            {{0, 0, 60, 60}, {0, 0, 0.8, 1},  "L1", NULL, NULL}, 
            {{100, 0, 20, 20}, {0.8, 0, 0.2, 0.4}, "R1", NULL, NULL}, 
            {{100, 10, 20, 20}, {0.8, 0.4, 0.2, 0.4}, "R2", NULL, NULL}, 
            {{100, 15, 20, 20}, {0.8, 0.8, 0.2, 0.2}, "RCMD", NULL, NULL}, 
            };


    for (i=0; i<4; i++) {
        win[i].win = newwin(win[i].locate.y, win[i].locate.x, 0, 0);
        if (NULL == win) {
            printf("Init failed.\n");
            ret = -1;
            break;
        }
        scrollok(win[i].win, TRUE);
        windows.insert(Windows::value_type(win[i].name, win[i]));
    }
 
    return ret;
}

static void sig_winch(int signo)
{
    resize_allwin();
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

    init_keyboard();
    initscr();

    init_allwin();
    resize_allwin();

    signal(SIGWINCH, sig_winch);

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
            // wprintw(win, ".");
            // wrefresh(win);
        }
        // wprintw(win, ".");
        // wrefresh(win);
    }

quit:
    close_keyboard();
    cleanup();

    for (int i = 30; i<129;i++) {
        printf("%d %c\t", i, i);
    }
    printf("\n");

    struct winsize size;
    Win *win = NULL;
    ioctl(STDIN_FILENO,TIOCGWINSZ,&size);
    Windows::iterator iter;
    for (iter=windows.begin(); iter!=windows.end(); iter++) {
        win = &iter->second;
        printf("%s @(%d,%d), (%dx%d)\n", win->name,
                win->locate.x, win->locate.y,
                win->locate.w, win->locate.h,
                win->title);
    }
    printf("SCREEN: %d x %d\n", size.ws_col, size.ws_row);

    return ret;
}
