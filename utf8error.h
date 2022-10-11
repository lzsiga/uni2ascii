/*
 *  Codes for errors from UTF-8 input routines.
 *  These values are all above the maximum valid UTF32 value.
 */

/* Normal end of file. Not really an error. */
#define UTF8_ENDOFFILE 0xA0000000
/* 
 * An initial byte is not followed by the required number of bytes 
 * Note that this code is returned only when the total number of following
 * bytes is insufficient, without regard to their type. If the total
 * number of following bytes is sufficient but the contiguous sequence
 * of continuation bytes is not long enough, the error UTF8_BADINCODE
 * is returned. 
 */
#define UTF8_NOTENOUGHBYTES 0xA0000001
/* 
 * The byte sequence is ill-formed. For example, the first byte might be a continuation
 * byte, or the sequence of continuation bytes following a valid first byte might
 * not be long enough. 
 */
#define UTF8_BADINCODE 0xA0000002
/* Decoding was successful but the resulting value is not a legal Unicode codepoint */
#define UTF8_BADOUTCODE 0xA0000003
/* An i/o error occured while attempting to read the input */
#define UTF8_IOERROR 0xA0000004


