/* Time-stamp: <2008-06-04 01:22:32 poser>
 *
 * This function reads from a file descriptor presumed to contain text encoded
 * in UTF-8 and returns the next UTF-32 character. It performs the same
 * conversion from UTF-8 to UTF-32 as the model function provided by the
 * Unicode consortium but is intended for reading from streams. For some
 * applications it may be more efficient to read large blocks of input
 * and then use the buffer conversion function, but this is not compatible
 * with reading from pipes.
 *
 * This version "returns" via pointers passed as arguments the number of bytes read
 * and a pointer to the raw byte string.
 * 
 * Author: Bill Poser (billposer@alum.mit.edu)
 * Patch by Dylan Thurston to resume read interrupted in mid-sequence.
 *
 * I place the code in this file in the public domain.
 */

#include <unistd.h>
#include <stdlib.h>
#include "unicode.h"
#include "utf8error.h"

#define FALSE 0
#define TRUE 1

/* 
 * The following are based on the Unicode consortium code. 
 */


/*
 * Index into the table below with the first byte of a UTF-8 sequence to
 * get the number of bytes that should follow.
 */

static const char TrailingBytesForUTF8[256] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};

/*
 * Magic values subtracted from a buffer value during UTF8 conversion.
 * This table contains as many values as there might be trailing bytes
 * in a UTF-8 sequence.
 */
static const UTF32 OffsetsFromUTF8[6] = { 0x00000000UL, 0x00003080UL, 0x000E2080UL, 
					 0x03C82080UL, 0xFA082080UL, 0x82082080UL };

/*
 * Utility routine to tell whether a sequence of bytes is legal UTF-8.
 * This must be called with the length pre-determined by the first byte.
 * If not calling this from ConvertUTF8to*, then the length can be set by:
 *	length = TrailingBytesForUTF8[*source]+1;
 */

static Boolean LegalUTF8P(const UTF8 *source, int length) {
  UTF8 a;
  const UTF8 *srcptr = source+length;
  switch (length) {
  default: return FALSE;
    /* Everything else falls through when "TRUE"... */
  case 4: if ((a = (*--srcptr)) < 0x80 || a > 0xBF) return FALSE;
  case 3: if ((a = (*--srcptr)) < 0x80 || a > 0xBF) return FALSE;
  case 2: if ((a = (*--srcptr)) > 0xBF) return FALSE;
    switch (*source) {
      /* no fall-through in this inner switch */
    case 0xE0: if (a < 0xA0) return FALSE; break;
    case 0xF0: if (a < 0x90) return FALSE; break;
    case 0xF4: if (a > 0x8F) return FALSE; break;
    default:  if (a < 0x80) return FALSE;
    }
  case 1: if (*source >= 0x80 && *source < 0xC2) return FALSE;
    if (*source > 0xF4) return FALSE;
  }
  return TRUE;
}

/* End of Unicode Consortium code */


UTF32 UTF8in (int fd, int *bytes, unsigned char **bstr)
{
  int BytesSoFar;
  int BytesRead;
  int BytesNeeded;		/* Additional bytes after initial byte */
  static unsigned char c[6];
  UTF32 ch;			/* Result character */

  unsigned char *cptr;

  cptr = &(c[0]);

  /* Get the first byte */
  BytesRead = read(fd,(void *) c,1);
  if (BytesRead == 0) {
    *bytes = 0;
    return (UTF8_ENDOFFILE);
  }
  if (BytesRead < 0) {
    *bytes = 0;
    return (UTF8_IOERROR); 
  }

  /* Now get the remaining bytes */
  BytesNeeded = (int) TrailingBytesForUTF8[c[0]];
  BytesSoFar = 0;
  do {
    BytesRead = read(fd,(void *) &c[BytesSoFar+1],(size_t) (BytesNeeded-BytesSoFar));
    BytesSoFar += BytesRead;
  } while (BytesRead > 0 || BytesSoFar < BytesNeeded);
  if(BytesSoFar != BytesNeeded) {
    *bytes = (BytesSoFar + 1);
    return(UTF8_NOTENOUGHBYTES);
  }
  *bytes = BytesNeeded+1;
  *bstr = &c[0];

  /* Check validity of source */
  if(! LegalUTF8P((void *) c,BytesNeeded+1)) return(UTF8_BADINCODE);

  ch = 0;
  switch (BytesNeeded) {
    case 5:	ch += *cptr++; ch <<= 6;
    case 4:	ch += *cptr++; ch <<= 6;
    case 3:	ch += *cptr++; ch <<= 6;
    case 2:	ch += *cptr++; ch <<= 6;
    case 1:	ch += *cptr++; ch <<= 6;
    case 0:	ch += *cptr++;
  }
  ch -= OffsetsFromUTF8[BytesNeeded];

  if(ch <= UNI_MAX_UTF32) return(ch);
  else return(UNI_REPLACEMENT_CHAR);
}
