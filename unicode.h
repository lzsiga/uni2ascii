/* unicode.h */

#ifndef UNICODE_H
#define UNICODE_H

#include <stdint.h>

typedef uint32_t      UTF32;   /* at least 32 bits */
typedef unsigned char UTF8;    /* 8 bits */
typedef unsigned char Boolean; /* 0 or 1 */

#define UNI_MAX_UTF32        ((UTF32)0x7FFFFFFFUL)
#define UNI_REPLACEMENT_CHAR ((UTF32)0x0000FFFDUL)

#define MIN_HS ((uint32_t)0xd800U)
#define MAX_HS ((uint32_t)0xdbffU)
#define MIN_LS ((uint32_t)0xdc00U)
#define MAX_LS ((uint32_t)0xdfffU)
#define MIN_BMP ((uint32_t)0)
#define MAX_BMP ((uint32_t)0xffffU)
#define MIN_SMP ((uint32_t)0x10000UL)
#define MAX_SMP ((uint32_t)0x10ffffUL)

#define IsHighSurrogate(x) (((uint32_t)(x) >= MIN_HS) && ((uint32_t)(x) <= MAX_HS))
#define IsLowSurrogate(x)  (((uint32_t)(x) >= MIN_LS) && ((uint32_t)(x) <= MAX_LS))
#define IsSurrogate(x)     (((uint32_t)(x) >= MIN_HS) && ((uint32_t)(x) <= MAX_LS))

#endif
