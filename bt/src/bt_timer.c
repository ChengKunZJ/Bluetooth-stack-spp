#include "bt_timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "trace.h"
#include <errno.h>
#include <string.h>



timer_t test_timerid;
timer_t bt_hci_reset_timerId;


void timer_handler(union sigval sv) {
    TRACE_INFO("Timer expired!");
    uTimer_delete(test_timerid);

    // timer_t *ptimer = (timer_t *)sv.sival_ptr;
    // TRACE_INFO("Timer expired!");
    // if (ptimer != NULL) {
    //     uTimer_delete(*ptimer);
    // }
}

int uTimer_create(timer_t *timerid, uint32_t time_ms, void (*timer_cb)(union sigval))
{
    // Define a structure to specify the timer notification behavior
    struct sigevent sev;
    // Define a structure to specify the timer's initial expiration and interval time
    struct itimerspec its;

    // Set the notification method when the timer expires
    sev.sigev_notify = SIGEV_THREAD;               // Notify via a new thread when the timer expires
    sev.sigev_notify_function = timer_cb;          // Function to call when the timer expires
    sev.sigev_value.sival_ptr = NULL;              // Parameter passed to the callback function via union sigval (NULL here)
    sev.sigev_notify_attributes = NULL;            // Use default thread attributes (e.g., stack size)

    // Create a new timer using the CLOCK_REALTIME clock and the specified notification settings
    if (timer_create(CLOCK_REALTIME, &sev, timerid) == -1) {
        TRACE_ERROR("timer_create fail");
        return -1;
    }

    // Set the timer to trigger once after the specified initial expiration time
    its.it_value.tv_sec = time_ms / 1000;               // Convert milliseconds to seconds
    its.it_value.tv_nsec = (time_ms % 1000) * 1000000;  // Convert remaining milliseconds to nanoseconds
    its.it_interval.tv_sec = 0;                         // Set interval to 0 for one-shot timer (no repeat)
    its.it_interval.tv_nsec = 0;

    // Start the timer; '0' indicates relative time (from now)
    if (timer_settime(*timerid, 0, &its, NULL) == -1) {
        TRACE_ERROR("timer_settime fail");
        return -2;
    }

    TRACE_INFO("timer create timerid = [%p]", (void *)*timerid);
    return 0;
}

int uTimer_delete(timer_t timerid)
{
    
    int ret = timer_delete(timerid);
    TRACE_INFO("timer_delete = [%d]", ret);
    if (ret == -1)
    {
        /* strerror(errno) is used to translate system errors into human-readable language. */
        TRACE_ERROR("timer_delete failed: %s", strerror(errno));
        return -1;
    }

    return ret;
}

void utimer_test()
{
    uTimer_create(&test_timerid, 1000, timer_handler);
}