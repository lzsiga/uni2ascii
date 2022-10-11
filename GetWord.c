#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

/* 
 * Read a chunk of arbitrary length from a file, terminated
 * by whitespace.
 *
 * Return a pointer to the null-terminated string allocated, or null on failure
 * to allocate sufficient storage.
 * The length of the word is placed in the variable pointed to by
 * the second argument.
 *
 * On EOF, the eof flag in the third command line argument is set.
 * It is the responsibility of the caller to free the space allocated.
 */

#define INITLENGTH 16


unsigned long GetWordLineNo;

char * Get_Word(FILE *fp, int *WordLength,int *eofptr)
{
  int c;
  int Available;
  int CharsRead;
  char *Word;

  Available = INITLENGTH;
  CharsRead=0;
  Word = (char *) malloc((size_t)Available);
  if(Word == NULL) return (Word);
  
  *eofptr = 0;
  while(1){
    c=getc(fp);
    //    if(isspace(c)) {
    if ( (c == ' ') || (c == '\t') || (c == '\n') || (c == '\r') || (c == '\f') || (c == '\v')) {
      if (c == '\n') GetWordLineNo++;
      Word[CharsRead]='\0';
      *WordLength=CharsRead;
      return(Word);
    }
    if(c == EOF){
      Word[CharsRead]='\0';
      *WordLength=CharsRead;
      *eofptr = 1;
      return(Word);
    }
    if(CharsRead == (Available-1)){ /* -1 because of null */
      Available += INITLENGTH/2;
      Word = (char *) realloc( (void *) Word, (size_t) (Available * sizeof (char)));
      if(Word == NULL) return(Word);
    }
    Word[CharsRead++]= (char) c;
  }
}
