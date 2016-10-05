#include "tty.h"

#include <ctype.h>
#include <string.h>

#include "intr.h"

void tty_init(tty *tp)
{
    memset(tp, 0, sizeof *tp);
}

int tty_read_char(tty *tp)
{
    int c = -1;
    WITH_INTERRUPTS_MASKED {
        size_t count = tp->read_count;
        if (count) {
            size_t head = tp->read_head;
            c = tp->read_buffer[head];
            head++;
            if (head >= TTY_READBUF_SIZE)
                head -= TTY_READBUF_SIZE;
            tp->read_head = head;
            tp->read_count--;
        }
    }
    return c;
}

// The TTY read buffer has two parts: chars that are ready to be read,
// and chars that are not because an EOL hasn't been received.  The
// latter are called "pre-read" and may be erased by BS, DEL or Ctrl-U.

static void enqueue_pre_read_char(tty *tp, char c)
{
    size_t count = tp->read_count + tp->pre_read_count;
    if (count < TTY_READBUF_SIZE) {
        size_t index = tp->read_head + count;
        if (index >= TTY_READBUF_SIZE)
            index -= TTY_READBUF_SIZE;
        tp->read_buffer[index] = c;
        tp->pre_read_count++;
    }
}

static void tty_backspace(tty *tp)
{
    tty_write_chars(tp, "\b \b", 3);
}

static void tty_echo(tty *tp, char c)
{
    char buf[1] = { c };
    tty_write_chars(tp, buf, 1);
}

void tty_receive_char(tty *tp, char c)
{
    size_t bs_count = 0;
    if (c >= ' ' && c < '\177') { // printing characters
        c = toupper(c);
        WITH_INTERRUPTS_MASKED {
            enqueue_pre_read_char(tp, c);
        }
        tty_echo(tp, c);
    } else if (c == '\r') {     // carriage return
        WITH_INTERRUPTS_MASKED {
            enqueue_pre_read_char(tp, '\n');
            tp->read_count += tp->pre_read_count;
            tp->pre_read_count = 0;
        }
        tty_echo(tp, '\n');
    } else if (c == '\b' || c == '\177') { // backspace, delete
        WITH_INTERRUPTS_MASKED {
            if (tp->pre_read_count) {
                tp->pre_read_count--;
                bs_count = 1;
            }
        }
    } else if (c == ('U' ^ 0100)) { // Ctrl-U: erase line.
        WITH_INTERRUPTS_MASKED {
            bs_count = tp->pre_read_count;
            tp->pre_read_count = 0;
        }
    } else {
        // ignore unknown chars
    }
    while (bs_count--) {
        tty_backspace(tp);
    }
}

void tty_receive_chars(tty *tp, const char *buf, size_t count)
{
    for (size_t i = 0; i < count; i++)
        tty_receive_char(tp, buf[i]);
}

void tty_register_send_callback(tty *tp, tty_send_callback *cb)
{
    tp->send_callback = cb;
}

void tty_write_chars(tty *tp, const char *buf, size_t size)
{
    if (tp->send_callback) {
        while (1) {
            const char *p = memchr(buf, '\n', size);
            size_t n = p ? (size_t)(p - buf) : size;
            (*tp->send_callback)(tp, buf, n);
            if (p) {
                (*tp->send_callback)(tp, "\r\n", 2);
                buf += n + 1;
                size -= n + 1;
            } else {
                break;
            }
        }
    }
}
