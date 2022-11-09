
#include "config.h"
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

#include "unicode.h"
#include "putu8.h"

/*
 * Takes a UTF-32 code as input and outputs UTF-8.
 * Based on  Roman Czyborra:  http://czyborra.com/utf/
 */

void fputu8(uint32_t c, FILE *f)
{
static const uint32_t lim1seq=     0x80UL; /*  7 bits */
static const uint32_t lim2seq=    0x800UL; /* 11 bits: 5+6 */
static const uint32_t lim3seq=  0x10000UL; /* 16 bits: 4+6+6 */
static const uint32_t lim4seq= 0x110000UL; /* 21 bits: 3+6+6+6 (cf RFC-3629) */

  if (c < lim1seq) {		/* ASCII */
    fputc(c, f);
  }
  else if (c < lim2seq) {
    fputc(0xC0 | c>>6, f);
    fputc(0x80 | (c & 0x3F), f);
  }
  else if (c < lim3seq) {
    fputc(0xE0 | c>>12, f);
    fputc(0x80 | (c>>6 & 0x3F), f);
    fputc(0x80 | (c & 0x3F), f);
  }
  else if (c < lim4seq) {
    fputc(0xF0 | c>>18, f);
    fputc(0x80 | (c>>12 & 0x3F), f);
    fputc(0x80 | (c>>6 & 0x3F), f);
    fputc(0x80 | (c & 0x3F), f);
  }
  else {
    fputc('?', stdout);
  }
}
