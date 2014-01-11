#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curses.h>
#include <term.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <time.h>
#include <locale.h>

int main()
{
    char ch;
    ch = readch();
    printf("%d\n", ch);
}
