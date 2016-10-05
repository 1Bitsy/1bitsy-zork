#include "tty-stdio.h"

#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>

#include "tty.h"

static ssize_t tty_read(void *cookie, char *buffer, size_t size)
{
    tty *tp = cookie;
    size_t n = 0;
    while (n < size) {
        int c = tty_read_char(tp);
        if (c == -1 && !n)
            continue;
        buffer[n++] = c;
        if (c == '\n')
            break;
    }
    return n;
}

static ssize_t tty_write(void *cookie, const char *buffer, size_t size)
{
    tty *tp = cookie;
    tty_write_chars(tp, buffer, size);
    return size;
}

void tty_stdio_init(tty *tp)
{
    static cookie_io_functions_t stdin_fns = {
        .read  = tty_read,
        .write = NULL,
        .seek  = NULL,
        .close = NULL,
    };
    stdin = fopencookie(tp, "r", stdin_fns);

    static cookie_io_functions_t stdout_fns = {
        .read  = NULL,
        .write = tty_write,
        .seek  = NULL,
        .close = NULL,
    };
    stdout = fopencookie(tp, "w", stdout_fns);
    (void)setvbuf(stdout, tp->write_buffer, _IOLBF, sizeof tp->write_buffer);

    static cookie_io_functions_t stderr_fns = {
        .read  = NULL,
        .write = tty_write,
        .seek  = NULL,
        .close = NULL,
    };
    stderr = fopencookie(tp, "w", stderr_fns);
    (void)setvbuf(stderr, NULL, _IONBF, 0);
}
