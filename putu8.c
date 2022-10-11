
#include "config.h"
#include <stdio.h>
#include <stddef.h>
#include <wchar.h>

/*
 * Takes a UTF-32 code as input and outputs UTF-8.
 * Based on  Roman Czyborra:  http://czyborra.com/utf/
 */

void
putu8(wchar_t c)
{
  if (c < 0x80) {		/* ASCII */
    putchar(c);
  }
  else if (c < 0x800) {
    putchar(0xC0 | c>>6);
    putchar(0x80 | (c & 0x3F));
  }
  else if (c < 0x10000) {
    putchar(0xE0 | c>>12);
    putchar(0x80 | (c>>6 & 0x3F));
    putchar(0x80 | (c & 0x3F));
  }
  else if (c < 0x200000) {	/* 2^21 */
    putchar(0xF0 | c>>18);
    putchar(0x80 | (c>>12 & 0x3F));
    putchar(0x80 | (c>>6 & 0x3F));
    putchar(0x80 | (c & 0x3F));
  }
}



