#ifndef BT_TIMER_H
#define BT_TIMER_H

#include <time.h>
#include <stdint.h>
#include <signal.h>

extern timer_t bt_hci_reset_timerId;


int uTimer_create(timer_t *timerid, uint32_t time_ms, void (*timer_cb)(union sigval));
int uTimer_delete(timer_t timerid);
void utimer_test();

#endif
