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

const char *file2str(const char *file_path)
{
    char *buffer = NULL;
    long file_size=0;
    FILE *f = NULL;

    f = fopen(file_path, "r");
    if (f)
    {
        fseek(f, 0, SEEK_END);
        file_size = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(file_size+1);
        if (buffer)
        {
            fread(buffer, 1, file_size, f);
            buffer[file_size] = '\0'; // sinon pas de EOF
        }
        fclose(f);
        return buffer;
    }
    return NULL;
}