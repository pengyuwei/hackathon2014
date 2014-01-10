#ifndef _WORKER_H
#define _WORKER_H

#include "c.h"

void lock_L1();
void unlock_L1();
void lock_R1();
void unlock_R1();
int start_work_thread(Win *L1, Win *R1);

int start_http_log();
int start_http_stat();
int start_free_mem();

#endif
