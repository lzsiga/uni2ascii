/* putu8.h */

#ifndef PUTU8_H
#define PUTU8_H

#include <stdio.h>

#include "unicode.h"

void fputu8(uint32_t chr, FILE *f);

#define putu8(c) fputu8((c),stdout)

#endif
