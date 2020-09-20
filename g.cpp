#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <unistd.h>

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
static int round = 0;

typedef struct _Object
{
    int x;
    int y;
    int life;
    char face;
    int score;
    int xstep;
    int ystep;
} Object;
typedef std::vector<Object> Monsters;
Monsters monsters;
Object me;

typedef std::vector<Object> Fires;
Fires fires;

static int rnd(int from, int to)
{
    return (rand() % (to - from + 1)) + from;
}

static void draw_me()
{
    mvwaddch(win_disp->win, me.y, me.x, me.face);
}

// static void draw_refresh()
// {
//     wrefresh(win_disp->win);
// }

static void draw()
{
    unsigned i = 0;

    if (NULL == win_disp) {
        return;
    }
    wclear(win_disp->win);
    char msg[16] = {0};
    sprintf(msg, "L%d R%d Score:%d", level, round, me.score);
    mvwaddstr(win_disp->win, 0, 1, msg);

    mvwaddch(win_disp->win, me.y, me.x, me.face);
    for (i=0; i<monsters.size(); i++) {
        Object *m = &monsters[i];
        mvwaddch(win_disp->win, m->y, m->x, m->face);
    }
    for (i=0; i<fires.size(); i++) {
        Object *f = &fires[i];
        mvwaddch(win_disp->win, f->y, f->x, f->face);
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
        m.score = 10;
        monsters.push_back(Monsters::value_type(m));
    }
}

static int fire_fly()
{
    int hit = 0;
    int ret = 0;

    for (unsigned i=0; i<fires.size(); i++) {
        ret++;
        hit = 0;
        Object *f = &fires[i];
        mvwaddch(win_disp->win, f->y, f->x, ' '); // erase old face
        f->x += f->xstep;
        f->y += f->ystep;
        if (f->x < 0 || f->x >= win_disp->locate.w
                || f->y < 0 || f->y >= win_disp->locate.h) {
            fires.erase(fires.begin() + i);
            break;
        }
        // hit test
        for (unsigned j=0; j<monsters.size(); j++) {
            Object *m = &monsters[j];
            if (f->x == m->x && f->y == m->y) {
                m->life--;
                if (m->life <= 0) {
                    hit = 1;
                    monsters.erase(monsters.begin() + j);
                    me.score += m->score;
                    break;
                }
            }
        }
        if (hit) {
            continue;
        }
        mvwaddch(win_disp->win, f->y, f->x, f->face);
        draw_me();
        wrefresh(win_disp->win);

        usleep(1000*100);
    }

    return ret;
}

static int monsters_round()
{
    int ret = 0;
    int yoffset = 0;
    int xoffset = 0;

    for (unsigned i=0; i<monsters.size(); i++) {
        ret++;
        Object *m = &monsters[i];
        mvwaddch(win_disp->win, m->y, m->x, ' ');
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
        if (m->y == me.y && m->x == me.x) {
            me.life--;
        }
        mvwaddch(win_disp->win, m->y, m->x, m->face);
        wrefresh(win_disp->win);
        usleep(1000*200);
    }
    
    return ret;
}

static void gameover()
{
    int pos = 1;
    wclear(win_disp->win);
    draw();
    mvwaddstr(win_disp->win, pos++, 1, "Game Over!");
    mvwaddstr(win_disp->win, pos++, 1, "No.  name   score ");
    mvwaddstr(win_disp->win, pos++, 1, "------------------");
    mvwaddstr(win_disp->win, pos++, 1 ,"1.   0xFF      190");
    mvwaddstr(win_disp->win, pos++, 1 ,"2.   0xFF      120");
    mvwaddstr(win_disp->win, pos++, 1 ,"3.   ....    .....");
    mvwaddstr(win_disp->win, pos++, 1 ,"4.   ....    .....");
    mvwaddstr(win_disp->win, pos++, 1 ,"5.   ....    .....");
    mvwaddstr(win_disp->win, pos++, 1 ,"6.   ....    .....");
    wrefresh(win_disp->win);

    readch();
    return;
}

static void new_fire(int xstep, int ystep)
{
    Object f;
    f.x = me.x;
    f.y = me.y;
    f.face = '.';
    f.xstep = xstep;
    f.ystep = ystep;
    fires.push_back(f);
}

static int do_round(char *key)
{
    unsigned i = 0;
    int ret = 0;
    int len = strlen(key);

    for (i=0; i<monsters.size(); i++) {
        ret++;
        Object *m = &monsters[i];
        m->score += len*10;
    }

    for (i=0; i<len; i++) {
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
                if (me.y < win_disp->locate.h) {
                    me.y++;
                }
                break;
            case 'd':
                if (me.x < win_disp->locate.w) {
                    me.x++;
                }
                break;
            case 'h':
                new_fire(-1, 0);
                break;
            case 'j':
                new_fire(0, 1);
                break;
            case 'k':
                new_fire(0, -1);
                break;
            case 'l':
                new_fire(1, 0);
                break;
            default:
                break;
        }
        ret = monsters_round();
        draw();
        
        fire_fly();
        if (me.life <= 0) {
            gameover();
            return -1;
        }

        if (ret <= 0) {
            while (fire_fly() > 0);
            break;
        }
        usleep(1000*500);
    }
    
    return ret;
}

// return value: 0:running; not 0:quit;
static int wait_user_cmd(char *keybuf, size_t buflen)
{
    int ch = 0;
    int cursor = 0;

    while(1) {
        ch = readch();
        keybuf[cursor++] = ch;
        if (ch == '\n' || cursor >= buflen - 1) {
            wclear(win_input->win);
            wrefresh(win_input->win);
            return cursor;
        } else if (ch == KEY_BACKSPACE || ch == 127) {
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
    };

    return cursor;
}

void game_main_loop(Win *disp, Win *input)
{
    int ret = 0;
    static char keybuf[32] = {0};

    win_disp = disp;
    win_input = input;

    me.x = win_disp->locate.w / 2;
    me.y = win_disp->locate.h / 2;
    me.life = 1;
    me.face = 'X';
    me.score = 0;
    round = 0;
    init_game(level);
    wclear(win_disp->win);
    draw();

    do {
        round++;
        memset(keybuf, 0, sizeof(keybuf));
        wait_user_cmd(keybuf, sizeof(keybuf));
        ret = do_round(keybuf);
        if (ret == -1) {
            break;
        }
        if (ret == 0) {
            round = 0;
            init_game(++level);
            continue;
        }
        if (keybuf[0] == 'q') {
            break;
        }
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
