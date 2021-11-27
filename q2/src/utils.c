#include "common.h"
#include "utils.h"

void err_n_die(const char *fmt, ...)
{
    int errno_save;
    va_list ap;

    errno_save = errno;

    fprintf(stderr, COLOR_RED);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    fflush(stderr);

    if (errno_save != 0)
    {
        fprintf(stderr, "(errno = %d) : %s\n", errno_save, strerror(errno_save));
        fprintf(stderr, "\n");
        fprintf(stderr, COLOR_RESET);
        fflush(stderr);
    }

    va_end(ap);
    exit(1);
}
