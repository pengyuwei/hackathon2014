#ifndef _WORKER_H
#define _WORKER_H

#include "c.h"

void lock_L1();
void unlock_L1();
int start_work_thread(Win *win);

int start_http_log();
int start_http_stat();
int start_free_mem();

#endif
