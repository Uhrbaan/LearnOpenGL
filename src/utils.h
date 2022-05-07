#ifndef utils_h
#define utils_h

#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

#define TERM_COL_ERROR(msg)     "\x1b[31;1m" msg "\x1b[0;0m"
#define TERM_COL_SUCCESS(msg)   "\x1b[32;1m" msg "\x1b[0;0m"
#define TERM_COL_INFO(msg)      "\x1b[35;1m" msg "\x1b[0;0m"

// sleeps for msec miliseconds
int msleep(int msec);

#endif