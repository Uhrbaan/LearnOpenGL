#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int msleep(long msec) // sleep for msec miliseconds
{
    struct timespec ts;
    int res;

    if (msec < 0)
        return -1;

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    res = nanosleep(&ts, &ts);

    return res;
}