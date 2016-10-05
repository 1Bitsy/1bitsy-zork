#!/usr/bin/env python3

TEXTFILE = 'dtextc.txt'

template = """
#ifndef TEXTARRAY_included
#define TEXTARRAY_included

#include <stdint.h>

/* This file generated automatically by {prog}.  Do not edit. */

static const uint8_t textarray[] = {{
XXX
}};

static const size_t textarray_size = sizeof textarray / sizeof *textarray;

#endif /* !defined(TEXTARRAY_included) */
"""

def main(argv):
    pre, post = template.strip().format(prog=argv[0]).split('XXX')
    with open(argv[1], 'rb') as f:
        print(pre)
        while True:
            b = f.read(8)
            if not b:
                break
            print('    {}'.format(' '.join('{:#04x},'.format(c) for c in b)))
        print(post)
            

if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))
