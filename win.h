#ifndef _WIN_H
#define _WIN_H

#include <curses.h>

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

void init_keyboard();
void close_keyboard();
void cleanup();
int kbhit();
int readch();

void lock_L1();
void unlock_L1();
void lock_R1();
void unlock_R1();

#endif
