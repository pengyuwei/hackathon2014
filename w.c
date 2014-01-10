#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <curses.h>
#include <string.h>

#include "w.h"


pthread_t tid_http_log;
pthread_mutex_t lock_http;

#define CMD_HTTP_LOG "tail -n 1 /usr/local/httpd/logs/error_log"
#define FILE_HTTP_LOG "/usr/local/httpd/logs/access_log"
#define CMD_HTTP_STAT "sh showlogerror.sh"

static int running = 1;
static Win *win_L1 = NULL;


void update_win(char *log)
{
    Win *win = win_L1;
    if (NULL == win) {
        printf("%s\n", log);
        return;
    }

    if (strlen(log) >= win->locate.w) {
        log[win->locate.w] = '\0';
    }

    wprintw(win->win, "%s", log);
    wrefresh(win->win);
}

void lock_L1()
{
    pthread_mutex_lock(&lock_http);
}

void unlock_L1()
{
    pthread_mutex_unlock(&lock_http);
}

void *http_log(void *arg)
{
    char *ret = NULL;
    FILE *fp = NULL;
    char buf[1024] = {0};\
    int lastpos = 0;

    while(running) {
        if (NULL == fp) {
            usleep(100);
            fp = fopen(FILE_HTTP_LOG, "r");
            fseek(fp, lastpos, SEEK_SET);
        }
        ret = fgets(buf, sizeof(buf), fp);
        lastpos = ftell(fp);
        if (NULL == ret) {
            fclose(fp);
            fp = NULL;
            continue;
        }
        lock_L1();
        update_win(buf);
        unlock_L1();
    }
    
/*
    printf("thread\n");
    fp = popen(CMD_HTTP_LOG, "r");
    while(running) {
        fread(buf, sizeof(char), sizeof(buf), fp);
        printf("%s\n", buf);
    }
*/
/*
    char buffer[1024] = {0};
    FILE *fp = NULL;
    int fd = 0;
    int wd = 0;
    fp = fopen(CMD_HTTP_LOG, "r");
    if (fp < 0) {
        goto quit;
    }  
    fd = inotify_init();
    wd = inotify_add_watch(fd, CMD_HTTP_LOG, IN_MODIFY);
    while(running) {
        read(fp, buffer, sizeof(buffer));  
        printf("%s\n", buffer);
        pthread_mutex_lock(&lock_http);
        pthread_mutex_unlock(&lock_http);
    }
quit:
*/

    return (void *)0;
}

void wait_work_thread()
{
    int ret = 0;
    void *tret;
    
    ret = pthread_join(tid_http_log, &tret);
    if (ret != 0) {
    }
}

int start_work_thread(Win *win)
{
    int ret = 0;

    win_L1 = win;
    
    start_http_log();
    start_http_stat();
    start_free_mem();
    
    return ret;
}

int start_http_log()
{
    int ret = 0;
    
    ret = pthread_create(&tid_http_log, NULL, http_log, NULL);
    
    return ret;
}

int start_http_stat()
{
    int ret = 0;
    
    return ret;
}

int start_free_mem()
{
    int ret = 0;
    
    return ret;
}

#ifdef DEBUG_MAIN
int main()
{  
    start_work_thread(NULL);
    
    sleep(3);
    running = 0;
    
    wait_work_thread();
    
    return 0;
}
#endif
