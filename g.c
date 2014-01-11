#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

#include "c.h"
#include "g.h"

/*
 * A text game for Linux
 * by yuwei5[yuwei5@staff.sina.com.cn]
 * 2014 SINA Hackathon
 * TOP10:
 * http://toyshop.sinaapp.com/game/top10.php
 * */

Win *win_disp = NULL;
Win *win_input = NULL;
static int level = 1;

typedef struct _Object
{
    int x;
    int y;
    int life;
    char face;
} Object;
typedef std::vector<Object> Monsters;
Monsters monsters;
Object me;

static int rnd(int from, int to)
{
    return (rand() % (to - from + 1)) + from;
}

static void draw()
{
    if (NULL == win_disp) {
        return;
    }
    wclear(win_disp->win);

    mvwaddch(win_disp->win, me.y, me.x, me.face);
    for (unsigned i=0; i<monsters.size(); i++) {
        Object *m = &monsters[i];
        mvwaddch(win_disp->win, m->y, m->x, m->face);
    }
    
    wrefresh(win_disp->win);
}

static void init_game(int level)
{
    int i = 0;

    monsters.clear();

    for (i=0; i<level; i++) {
        Object m;
        m.x = rnd(1, 10);
        m.y = rnd(1, 10);
        m.life = 1;
        m.face = '@';
        monsters.push_back(Monsters::value_type(m));
    }
}

static void monsters_round()
{
    int yoffset = 0;
    int xoffset = 0;

    for (unsigned i=0; i<monsters.size(); i++) {
        Object *m = &monsters[i];
        yoffset = me.y - m->y;
        xoffset = me.x - m->x;
        if (abs(yoffset) > abs(xoffset)) {
            if (yoffset > 0) {
                m->y++;
            } else {
                m->y--;
            }
        } else {
            if (xoffset > 0) {
                m->x++;
            } else {
                m->x--;
            }
        }
        mvwaddch(win_disp->win, m->y, m->x, m->face);
        draw();
    }
    
}

static void do_round(char *key)
{
    wclear(win_disp->win);

    for (int i=0; i<strlen(key); i++) {
        char cmd = key[i];
        switch(cmd) {
            case 'w':
                if (me.y > 1) {
                    me.y--;
                }
                break;
            case 'a':
                if (me.x > 1) {
                    me.x--;
                }
                break;
            case 's':
                if (me.y < 10) {
                    me.y++;
                }
                break;
            case 'd':
                if (me.x < 10) {
                    me.x++;
                }
                break;
            case 'h':
                break;
            case 'j':
                break;
            case 'k':
                break;
            case 'l':
                break;
            default:
                break;
        }
        draw();
        monsters_round();
    }
}

// return value: 0:running; not 0:quit;
static int wait_user_cmd(char *keybuf)
{
    int ch = 0;
    int cursor = 0;

    do {
        ch = readch();
        keybuf[cursor++] = ch;
        if (ch == '\n' || cursor >= (int)sizeof(keybuf) - 1) {
            return cursor;
        } else if (ch == KEY_BACKSPACE) {
            keybuf[cursor] = '\0';
            cursor--;
            if (cursor < 0) {
                cursor = 0;
            }
        }
        if (NULL != win_input) {
            wclear(win_input->win);
            mvwaddstr(win_input->win, 1, 1, keybuf);
            wrefresh(win_input->win);
        }
    } while (strstr(keybuf, "quit") == NULL);

    return cursor;
}

void game_main_loop(Win *disp, Win *input)
{
    static char keybuf[32] = {0};

    win_disp = disp;
    win_input = input;

    me.x = win_disp->locate.w / 2;
    me.y = win_disp->locate.h / 2;
    me.life = 1;
    me.face = 'X';
    init_game(level);

    do {
        draw();
        memset(keybuf, 0, sizeof(keybuf));
        wait_user_cmd(keybuf);
        do_round(keybuf);
    } while (strncmp(keybuf, "quit", 4) != 0);
}

#ifdef DEBUG_MAIN
int main()
{
    char ch;

    init_keyboard();
    init_game(1);

    do {
        ch = readch();
        printf("%c %d\n", ch, ch);
    } while (ch != 'q');

    close_keyboard();
    return 0;
}
#endif
