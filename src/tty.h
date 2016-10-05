#ifndef TTY_included
#define TTY_included

#include <stddef.h>

#define TTY_LINEBUF_SIZE 100
#define TTY_READBUF_SIZE 100

typedef struct tty tty;

typedef void tty_send_callback(tty *, const char *, size_t);

struct tty {
    volatile size_t    read_head;
    volatile size_t    read_count;
    volatile size_t    pre_read_count;
    tty_send_callback *send_callback;
    char               read_buffer[TTY_READBUF_SIZE];
    char               write_buffer[TTY_LINEBUF_SIZE];
};

// Application API
extern void tty_init(tty *);
extern void tty_write_chars(tty *, const char *, size_t);
extern int tty_read_char(tty *);

// Device driver API
extern void tty_receive_char(tty *, char);
extern void tty_receive_chars(tty *, const char *, size_t);
extern void tty_register_send_callback(tty *, tty_send_callback *);

#endif /* !TTY_included */
