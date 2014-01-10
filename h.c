#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include <curses.h>
#include <term.h>
#include <sys/ioctl.h>
#include <map>
#include <string>
#include <sys/wait.h>
#include <time.h>
#include <locale.h>

#include "c.h"
#include "w.h"

/*
by pyw 2014-1-9
gcc h.c -g -O0 -lncurses -I/usr/include/ncurses -o h
*/

typedef std::map<std::string, Win> Windows;
Windows frames; // all frames
Windows windows; // all content windows


Win *get_frame(const char *name)
{
    Win *win = NULL;
    Windows::iterator iter;

    iter = frames.find(name);
    if (iter == frames.end()) {
        return NULL;
    }
    win = &iter->second;
   
    return win;
}

Win *get_win(const char *name)
{
    Win *win = NULL;
    Windows::iterator iter;

    iter = windows.find(name);
    if (iter == windows.end()) {
        return NULL;
    }
    win = &iter->second;
   
    return win;
}

void resize_allwin()
{
    Win *win = NULL;
    Win *frame = NULL;
    Windows::iterator iter;
    struct winsize size;
    // char msg[64] = {0};
    int ret = 0;

    ioctl(STDIN_FILENO,TIOCGWINSZ,&size);

    for (iter=windows.begin(); iter!=windows.end(); iter++) {
        win = &iter->second;
        frame = get_frame(win->name);
        if (NULL == frame) {
            cleanup();
            exit(0);
        }
        // set new size
        frame->locate.x = size.ws_col * frame->scale.x;
        frame->locate.y = size.ws_row * frame->scale.y;
        frame->locate.w = size.ws_col * frame->scale.w;
        frame->locate.h = size.ws_row * frame->scale.h;

        win->locate.x = frame->locate.x + 1;
        win->locate.y = frame->locate.y + 1;
        win->locate.w = frame->locate.w - 2;
        win->locate.h = frame->locate.h - 2;

        if (iter->first.compare("RCMD") == 0) {
            win->locate.h = 4;
        }

        // resize 
        ret = wresize(frame->win, frame->locate.h, frame->locate.w);
        ret = wresize(win->win, win->locate.h, win->locate.w);
        if (ERR == ret) {
            printf("wresize failed:%s %d,%d\n", win->name, win->locate.x, win->locate.y);
        }

        // move
        ret = mvwin(frame->win, frame->locate.y, frame->locate.x);
        if (ERR == ret) {
            cleanup();
            printf("mvwin frame failed:%s %d,%d\n", frame->name, frame->locate.y, frame->locate.x);
            printf("\t %d x %d\n", frame->locate.w, frame->locate.h);
            exit(0);
        }
        ret = mvwin(win->win, win->locate.y, win->locate.x);
        if (ERR == ret) {
            cleanup();
            printf("SCREEN=%d x %d\n", COLS, LINES);
            printf("mvwin failed:%s %d,%d\n", win->name, win->locate.x, win->locate.y);
            printf("\t %d x %d\n", win->locate.w, win->locate.h);
            printf("frame %d,%d %d x %d\n", frame->locate.x, frame->locate.y, frame->locate.w, frame->locate.h);
            exit(0);
        }

        //echo();
        //touchwin(win->win);
        //attron(A_REVERSE);
        box(frame->win, '|', '-');
        //mvaddstr(2, 2,win->name);
        // mvwaddstr(win->win, 1, 1, win->name);
        //sprintf(msg, "%d,%d", win->locate.y, win->locate.x);
        //mvwaddstr(win->win, 2, 1, msg);
        //attroff(A_REVERSE);
        //move(1, 1);
        //waddstr(win->win, win->name);
        wrefresh(frame->win);
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
            {{0, 0, 60, 60}, {0, 0, 0.8, 1}, "L1", "", NULL}, 
            {{100, 0, 20, 20}, {0.8, 0, 0.2, 0.4}, "R1", "", NULL}, 
            {{100, 10, 20, 20}, {0.8, 0.4, 0.2, 0.4}, "R2", "", NULL}, 
            {{100, 15, 20, 20}, {0.8, 0.8, 0.2, 0.2}, "RCMD", "", NULL}, 
            };


    for (i=0; i<4; i++) {
        win[i].win = newwin(win[i].locate.y, win[i].locate.x, 0, 0);
        if (NULL == win) {
            printf("Init failed.\n");
            ret = -1;
            break;
        }
        if (strcmp(win[i].name, "L1") == 0) {
            scrollok(win[i].win, TRUE);
        } else {
            clearok(win[i].win, TRUE);
        }
        windows.insert(Windows::value_type(win[i].name, win[i]));

        Win frame;
        memcpy(&frame, &win[i], sizeof(Win));
        frame.win = newwin(win[i].locate.y, win[i].locate.x, 0, 0);
        frames.insert(Windows::value_type(win[i].name, frame));
    }
 
    return ret;
}

static void sig_winch(int signo)
{
    resize_allwin();
}

void update_allwin()
{
    time_t t;
    Win *win = get_win("L1");
    if (NULL == win) {
        return;
    }

    time(&t);

    lock_L1();
    wprintw(win->win, "%d\n", t);
    wrefresh(win->win);
    unlock_L1();
}

void clear_R2()
{
    Win *win = get_win("R2");
    if (NULL == win) {
        return;
    }
    wclear(win->win);
    wrefresh(win->win);
}

void show_weibo()
{
    FILE *fp = NULL;
    char buf[1024] = {0};\
    char *ret = NULL;

    fp = fopen("./weibo", "r");
    
    Win *win = get_win("R2");
    if (NULL == win) {
        return;
    }
    do {
        ret = fgets(buf, sizeof(buf), fp);
        wprintw(win->win, "%s\n", buf);
    } while (ret != NULL);
    wrefresh(win->win);
    
    fclose(fp);
    fp = NULL;
}

void play_game()
{

}

void process_command(char *cmd)
{
    if (strncmp(cmd, "weibo", 5) == 0) {
        show_weibo();
    } else if (strncmp(cmd, "game", 4) == 0){
        play_game();
    } else if (strncmp(cmd, "clear", 5) == 0){
        clear_R2();
    } else if (strncmp(cmd, "clean", 5) == 0){
        clear_R2();
    }
}

// return value: 0:running; not 0:quit;
int on_keypress()
{
    int ch = 0;
    static char keybuf[32] = {0};
    static int cursor = 0;

    Win *win = get_win("RCMD");
    if (NULL == win) {
        return 0;
    }

    ch = readch();
    keybuf[cursor++] = ch;
    if (ch == '\n' || cursor >= sizeof(keybuf) - 1) {
        cursor = 0;
        process_command(keybuf);
        memset(keybuf, 0, sizeof(keybuf));
        werase(win->win);
        wclear(win->win);
        mvwaddch(win->win, 1, 1, '> ');
        wrefresh(win->win);
    } else {
        waddch(win->win, ch);
        wrefresh(win->win);
    }

    return ch != 'q';
}

void cleanup_allwin()
{

}

int work_thread_start()
{
    Win *L1 = get_win("L1");
    Win *R1 = get_win("R1");

    start_work_thread(L1, R1);
    return 0;
}

int main_loop()
{
    int running = 1;

    while(running) {
        // update_allwin();
        usleep(100);
        if(kbhit()) {
            running = on_keypress();
        }
    }

    cleanup_allwin();

    return 0;
}

int main(int argc, char *argv[])
{
    int ret = 0;

    setlocale(LC_ALL,"");

    init_keyboard();
    initscr();

    init_allwin();
    resize_allwin();

    signal(SIGWINCH, sig_winch);

    work_thread_start();
    main_loop();

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
        printf("%s @(%d,%d), (%dx%d) %s\n", win->name,
                win->locate.x, win->locate.y,
                win->locate.w, win->locate.h,
                win->title);
    }
    printf("SCREEN: %d x %d\n", size.ws_col, size.ws_row);

    return ret;
}
