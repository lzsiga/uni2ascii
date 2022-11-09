/* Time-stamp: <2010-12-12 19:41:51 poser>
 *
 * Copyright (C) 2008 William J. Poser (billposer@alum.mit.edu)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 3 of the GNU General Public License as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 * or go to the web page:  http://www.gnu.org/licenses/gpl.txt.
 */

/* This file contains SetFormat, the function that determines the ascii format to be read or
 * written, the associated help function ListFormatArguments, and CountSlots, which is
 * used to validate user-supplied formats.
 */

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#ifdef HAVE_LIBINTL_H
#include <libintl.h>
#define _(String) gettext(String)
#else
#define _(x) (x)
#endif
#include <regex.h>
#include "formats.h"
#include "exitcode.h"


/* Ancillary function */

int mystrcasecmp(char *a, char *b) {
  char ac;
  char bc;
  while(*a != '\0') {
    ac = tolower(*a++);
    bc = tolower(*b++);
    if(ac < bc) return (-1);
    if(ac > bc) return (1);
  }
  if(*b != '\0') return (-1);
  return (0);
}

/* This is the public function. Note that only the first argument is an input parameter.
 * The other arguments are output parameters.
*/

void SetFormat(char *fs,
	       int *FType,
	       short *UseEntitiesP,
	       int *UTF8Type,
	       short *BMPSplitP,
	       short *AllHTMLP
	       )
{
  regex_t rx;
  
  /* First, check for old flag characters */

  if(strlen(fs) == 1) {
    switch(fs[0]) {
    case 'O':
      *FType = BYTEO;
      return;
    case 'S':
      *FType = BYTEH;
      return;
    case 'T':
      *FType = BYTED;
      return;
    case 'A':
      *FType = ABUX;
      return;
    case 'B':
      *FType = BSLX;
      return;
    case 'C':
      *FType = BSLXB;
      return;
    case 'D':
      *FType = HTMLD;
      return;
    case 'E':
      *FType = JUUX;
      return;
    case 'F':
      *FType = JuUX;
      return;
    case 'G':
      *FType = XQ;
      return;
    case 'H':
      *FType = HTMLX;
      return;
    case 'I':
      *FType = IFMT;
      *UTF8Type = 2;
      return;
    case 'J':
      *FType = JFMT;
      *UTF8Type = 1;
      return;
    case 'K':
      *FType = KFMT;
      *UTF8Type = 3;
      return;
    case 'L':
      *FType = BSLU;
      *BMPSplitP = 1;
      return;
    case 'M':
      *FType = SGMLX;
      return;
    case 'N':
      *FType = SGMLD;
      return;
    case 'P':
      *FType = UPLX;
      return;
    case 'Q':
      *FType = CHENT;
      *UseEntitiesP = 1;
      return;
    case 'R':
      *FType = RAWX;
      return;
    case 'U':
      *FType = BSLU;
      *BMPSplitP = 0;
      return;
    case 'V':
      *FType = BSLUD;
      *BMPSplitP = 0;
      return;
      /*
    case 'W':
      *FType = HDML;
      return;
      */
    case 'X':
      *FType = STDX;
      return;
    case 'Y':
      *FType = CHENT;
      *AllHTMLP = 1;
      return;
    case '0':
      *FType = UTF8ANGLE;
      *UTF8Type = 5;
      return;
    case '1':
      *FType = CLSX;
      return;
    case '2':
      *FType = PERLV;
      return;
    case '3':
      *FType = DOLLAR;
      return;
    case '4':
      *FType = PSPT;
      return;
    case '5':
      *FType = CLR; 
      return;
    case '6':
      *FType = ADA; 
      return;
    case '7':
      *FType = APACHE;
      *UTF8Type = 4;
      return;
    case '8':
      *FType = OOXML;
      return;
    case '9':
      *FType = PCTUX;
      return;
    default:
      fprintf(stderr,"Unknown format %s\n",fs);
      exit(BADOPTIONARG);
    }
  }

  /* Now look for fixed strings */

  if(mystrcasecmp(fs,"RFC2396") == 0) {*FType = JFMT;*UTF8Type = 1;return;}
  if( (mystrcasecmp(fs,"RFC2045") == 0) || 
      (mystrcasecmp(fs,"quoted_printable") == 0)) {*FType = IFMT; *UTF8Type = 2;return;}
  /*  if(mystrcasecmp(fs,"HDML") == 0) {*FType = HDML;return;} */
  if(mystrcasecmp(fs,"HTML_hexadecimal") == 0) {*FType = HTMLX;return;}
  if(mystrcasecmp(fs,"HTML_decimal") == 0) {*FType = HTMLD;return;}
  if(mystrcasecmp(fs,"SGML_hexadecimal") == 0) {*FType = SGMLX;return;}
  if(mystrcasecmp(fs,"SGML_decimal") == 0) {*FType = SGMLD;return;}
  if(mystrcasecmp(fs,"ADA") == 0) {*FType = ADA;return;}
  if(mystrcasecmp(fs,"Python") == 0) {*FType = BSLU;return;}
  if(mystrcasecmp(fs,"RTF") == 0) {*FType = BSLUD;*BMPSplitP = 0;return;}
  if(mystrcasecmp(fs,"Unicode") == 0) {*FType = UPLX;return;}
  if(mystrcasecmp(fs,"Apache") == 0) {*FType = APACHE;*UTF8Type=4;return;}
  if(mystrcasecmp(fs,"OOXML") == 0) {*FType = OOXML;return;}


  /* Match regexps against examples */

  regcomp(&rx,"^&#[xX][[:xdigit:]]{4,};$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED);	/* HTMLX */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = HTMLX;return;}
  regfree(&rx);

  regcomp(&rx,"^&#[[:digit:]]{4,};$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED); 	/* HTMLD */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = HTMLD;return;}
  regfree(&rx);

  regcomp(&rx,"^[\\]#[xX][[:xdigit:]]{4,};$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED);/* SGMLX */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = SGMLX;return;}
  regfree(&rx);

  regcomp(&rx,"^[\\]#[[:digit:]]{4,};$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED);	/* SGMLD */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = SGMLD;return;}
  regfree(&rx);

  /*  regcomp(&rx,"^&#[[:xdigit:]]{2};$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED);	HDML */
  /*  if(!regexec(&rx,fs,0,NULL,0)) {*FType = HDML;return;} */
  /*  regfree(&rx); */

  regcomp(&rx,"^[\\]u[[:xdigit:]]{4,}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED); 	/* BSLU */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = BSLU;return;}
  regfree(&rx);

  regcomp(&rx,"^[\\][xX][[:xdigit:]]{4,}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED);	/* BSLX */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = BSLX;return;}
  regfree(&rx);

  regcomp(&rx,"^0x[[:xdigit:]]{4,}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED);	/* STDX */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = STDX;return;}
  regfree(&rx);

  regcomp(&rx,"^#x[[:xdigit:]]{4,}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED);	/* CLSX */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = CLSX;return;}
  regfree(&rx);

  regcomp(&rx,"^[[:xdigit:]]{4,}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED);	/* RAWX */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = RAWX;return;}
  regfree(&rx);

  regcomp(&rx,"^[\\]x[{][[:xdigit:]]{4,}[}]$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED); /* BSLXB */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = BSLXB;return;}
  regfree(&rx);

  regcomp(&rx,"^<U[[:xdigit:]]{4,}>$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED); /* ABUX */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = ABUX;return;}
  regfree(&rx);

  regcomp(&rx,"^U[[:xdigit:]]{4,}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED); /* JUUX */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = JUUX;return;}
  regfree(&rx);

  regcomp(&rx,"^u[[:xdigit:]]{4,}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED); /* JuUX */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = JuUX;return;}
  regfree(&rx);

  regcomp(&rx,"^%u[[:xdigit:]]{4,}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED); /* PCTUX */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = PCTUX;return;}
  regfree(&rx);

  regcomp(&rx,"^U\\+[[:xdigit:]]{4,}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED); /* UPLX */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = UPLX;return;}
  regfree(&rx);

  regcomp(&rx,"^X\'[[:xdigit:]]{4,}\'$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED); /* XQ */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = XQ;return;}
  regfree(&rx);

  regcomp(&rx,"^[\\]u[[:digit:]]{4,}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED); /* BSLUD */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = BSLUD;return;}
  regfree(&rx);

  regcomp(&rx,"^v[[:digit:]]{4,}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED); /* PERLV */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = PERLV;return;}
  regfree(&rx);

  regcomp(&rx,"^[$][[:xdigit:]]{4,}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED); /* DOLLAR */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = DOLLAR;return;}
  regfree(&rx);

  regcomp(&rx,"^16#[[:xdigit:]]{4,}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED); /* PSPT */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = PSPT;return;}
  regfree(&rx);

  regcomp(&rx,"^#16r[[:xdigit:]]{4,}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED); /* CLR */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = CLR;return;}
  regfree(&rx);

  regcomp(&rx,"^16#[[:xdigit:]]{4,}#$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED); /* ADA */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = ADA;return;}
  regfree(&rx);

  regcomp(&rx,"^[\\][0-7]{3}[\\][0-7]{3}[\\][0-7]{3}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED); /* BYTEO */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = BYTEO;return;}
  regfree(&rx);

  regcomp(&rx,"^[\\]d[0-9]{3}[\\]d[0-9]{3}[\\]d[0-9]{3}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED); /* BYTED */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = BYTED;return;}
  regfree(&rx);

  regcomp(&rx,"^[\\]x[[:xdigit:]]{2}[\\]x[[:xdigit:]]{2}[\\]x[[:xdigit:]]{2}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED); /* BYTEH */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = BYTEH;return;}
  regfree(&rx);

  regcomp(&rx,"^(<[[:xdigit:]]{2}>){1,3}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED);	/* UTF8ANGLE */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = UTF8ANGLE;return;}
  regfree(&rx);

  regcomp(&rx,"^&[^0-9]+;$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED);	/* Non-numeric Character entity */
  if(!regexec(&rx,fs,0,NULL,0)) {*UseEntitiesP = 1;return;}
  regfree(&rx);

  regcomp(&rx,"^(=[[:xdigit:]]{2}){1,3}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED);	/* I format */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = IFMT;*UTF8Type=2;return;}
  regfree(&rx);

  regcomp(&rx,"^(%[[:xdigit:]]{2}){1,3}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED);	/* J format */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = JFMT;*UTF8Type=1;return;}
  regfree(&rx);

  regcomp(&rx,"^([\\][0-7]{3}){1,3}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED);	/* K format */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = KFMT;*UTF8Type=3;return;}

  regcomp(&rx,"^([\\][xX][[:xdigit:]]{2}){1,3}$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED);	/* Apache */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = APACHE;*UTF8Type=4;return;}
  regfree(&rx);

  regcomp(&rx,"^_[xX][[:xdigit:]]{1,6}_$",REG_NEWLINE|REG_NOSUB|REG_EXTENDED);	/* OOXML */
  if(!regexec(&rx,fs,0,NULL,0)) {*FType = OOXML;return;}
  regfree(&rx);


  regfree(&rx);
  *FType = FMT_UNKNOWN;
}

int CountSlots(char *s) {
  char PreviousChar = '\0';
  char c;
  char *p;
  int cnt = 0;

  p = s;
  while( (c=*p++) != '\0') {
    if(c == '%') {
      if(PreviousChar != '%') cnt++;
    }
    PreviousChar = c;
  }
  return(cnt); 
}

void OldListFormatArguments (short ToAsciiP) {
  fprintf(stderr, "The argument to the -a option may be be a format name, an example,\n");
  fprintf(stderr, "or an arbitrary single character specifier.\n");
  fprintf(stderr,
	  _(" 1 Common Lisp format hexadecimal numbers (#x00E9)\n"));
  fprintf(stderr,
	  _(" 2 Perl style decimal numbers with prefix v (v233)\n"));
  fprintf(stderr,
	  _(" 3 hexadecimal numbers with prefix $ ($00E9)\n"));
  fprintf(stderr,
	  _(" 4 hexadecimal numbers with prefix 16# (16#00E9)\n"));
  fprintf(stderr,
	  _(" 5 hexadecimal numbers with prefix #16r (#16r00E9)\n"));
  fprintf(stderr,
	  _(" 6 hexadecimal numbers with prefix 16# and suffix # (16#00E9#)\n"));
  fprintf(stderr,
	  _(" A hexadecimal numbers with prefix U in anglebrackets(<U00E9>)\n"));
  fprintf(stderr,
	  _(" B backslash-x escaped hexadecimal numbers (\\x00E9)\n"));
  fprintf(stderr,
	  _(" C backslash-x escaped hexadecimal numbers in braces (\\x{00E9})\n"));
  fprintf(stderr,
	  _(" D decimal numeric character references (&#0233;)\n"));
  fprintf(stderr,
	  _(" E hexadecimal with prefix U (U00E9)\n"));
  fprintf(stderr,
	  _(" F hexadecimal with prefix u (u00E9)\n"));
  fprintf(stderr,
	  _(" G hexadecimal in single quotes with prefix X (X\'00E9\')\n"));
  fprintf(stderr,
	  _(" H hexadecimal numeric character references (&#x00E9;)\n"));
  fprintf(stderr,
	  _(" I hexadecimal UTF-8 with each byte's hex preceded by an =-sign (=C3=A9).\n\t\tThis is the Quoted Printable format defined by RFC 2045.\n"));
  fprintf(stderr,
	  _(" J hexadecimal UTF-8 with each byte's hex preceded by a %%-sign  (%%C3%%A9)\n\t\tThis is the URI escape format defined by RFC 2396.\n"));
  fprintf(stderr,
	  _(" K octal UTF-8 with backslash escapes (\303\251)\n"));
  fprintf(stderr,
	  _(" L \\U-escaped hex outside the BMP (U+0000-U+FFFF), \\u-escaped hex within.\n"));
  fprintf(stderr,
	  _(" M hexadecimal SGML numeric character references (\\#x00E9;)\n"));
  fprintf(stderr,
	  _(" N decimal SGML numeric character references (\\#0233;)\n"));
  fprintf(stderr,
	  _(" O octal escapes for the three low bytes in big-endian order (\\000\\000\\351)\n"));
  fprintf(stderr,
	  _(" P hexadecimal numbers with prefix U+ (U+00E9)\n"));
  fprintf(stderr,
	  _(" Q character entities where possible (&eacute;)\n"));
  fprintf(stderr,
	  _(" R raw hexadecimal numbers (00E9)\n"));
  fprintf(stderr,
	  _(" S hexadecimal escapes for the three low bytes in big-endian order (\\x00\\x00\\xE9)\n"));
  fprintf(stderr,
	  _(" T decimal escapes for the three low bytes in big-endian order (\\d000\\d000\\d233)\n"));
  fprintf(stderr,
	  _(" U \\u-escaped hex (\\u00E9)\n"));
  fprintf(stderr,
	  _(" V \\u-escaped decimal (\\u0233)\n"));
  /*  fprintf(stderr,
      _(" W HDML two hexadecimal digits (&#E9;)\n")); */
  fprintf(stderr,
	  _(" X standard form hexadecimal numbers (0x00E9)\n"));
  if(!ToAsciiP) {
    fprintf(stderr,
	    _(" Y all three types of HTML  escape:  hexadecimal  character references,\ndecimal  character  references, and character entities\n"));
  }
}

void ListFormatArguments (short ToAsciiP) {
  fprintf(stderr, "The argument to the -a option may be be a format name, an example,\n");
  fprintf(stderr, "or an arbitrary single character specifier.\n");
  fprintf(stderr, _(" raw hexadecimal numbers\n"));
  fprintf(stderr,"\t(00E9)\tR\n");
  fprintf(stderr, _(" standard form hexadecimal numbers\n"));
  fprintf(stderr,"\t(0x00E9)\tX\n");
  fprintf(stderr, _(" prefix v decimal (Perl format)\n"));
  fprintf(stderr,"\t(v233)\t2\n");
  fprintf(stderr, _(" prefix $ hexadecimal ($00E9)\t3\n"));
  fprintf(stderr, _(" prefix 16# hexadecimal (16#00E9)\t4\n"));
  fprintf(stderr, _(" prefix #x hexadecimal (Common Lisp format) (#x00E9)\t1\n"));
  fprintf(stderr, _(" prefix #16r hexadecimal (#16r00E9)\t5\n"));
  fprintf(stderr, _(" prefix \\u decimal (\\u0233)\tV\n"));
  fprintf(stderr, _(" prefix \\u hexadecimal (\\u00E9)\tU\n"));
  fprintf(stderr, _(" prefix \\U outside BMP, \\u within, hexadecimal (U+0000-U+FFFF)\tL\n"));
  fprintf(stderr, _(" prefix U hexadecimal (U00E9)\tE\n"));
  fprintf(stderr, _(" prefix u hexadecimal (u00E9)\tF\n"));
  fprintf(stderr, _(" prefix %%u hexadecimal (%%u00E9)\t9\n"));
  fprintf(stderr, _(" prefix U+ hexadecimal (U+00E9)\tP\n"));
  fprintf(stderr, _(" prefix X with hexadecimal in single quotes (X\'00E9\')\tG\n"));
  fprintf(stderr, _(" prefix 16# and suffix # hexadecimal (16#00E9#)\t6\n"));
  fprintf(stderr, _(" prefix U in anglebrackets hexadecimal (<U00E9>)\tA\n"));
  fprintf(stderr, _(" prefix backslash-x hexadecimal (\\x00E9)\tB\n"));
  fprintf(stderr, _(" prefix backslash-x hexadecimal in braces (\\x{00E9})\tC\n"));
  fprintf(stderr, _(" HTML numeric character references - decimal (&#0233;)\tD\n"));
  fprintf(stderr, _(" HTML numeric character references - hexadecimal (&#x00E9;)\tH\n"));
  fprintf(stderr, _(" SGML numeric character references -decimal (\\#0233;)\tN\n"));
  fprintf(stderr, _(" SGML numeric character references - hexadecimal (\\#x00E9;)\tM\n"));
  /*  fprintf(stderr, _(" HDML two hexadecimal digits (&#E9;)\tW\n")); */
  fprintf(stderr, _(" octal escapes for 3 low bytes in big-endian order (\\000\\000\\351)\tO\n"));
  fprintf(stderr, _(" hexadecimal escapes for 3 low bytes in big-endian order\n"));
  fprintf(stderr,"\t(\\x00\\x00\\xE9)\tS\n");
  fprintf(stderr, _(" decimal escapes for 3 low bytes in big-endian order (\\d000\\d000\\d233)\tT\n"));
  fprintf(stderr, _(" hexadecimal UTF-8 with each byte's hex preceded by an =-sign (=C3=A9).\n\t\tRFC 2045 Quoted Printable.\tI\n"));
  fprintf(stderr, _(" hexadecimal UTF-8 with each byte's hex preceded by a %%-sign  (%%C3%%A9)\n\t\tRFC 2396  URI escape format.\tJ\n"));
  fprintf(stderr, _(" hexadecimal UTF-8 with each byte's hex preceded by a backslash-x  (\\xC3\\xA9)\n\t\tApache log format.\t7\n"));
  fprintf(stderr, _(" hexadecimal UTF-8 with each byte's hex surrounded by angle brackets  (<C3><A9>)\n\t\t\t0\n"));
  fprintf(stderr, _(" octal UTF-8 with backslash escapes (\303\251)\tK\n"));

  if(ToAsciiP) fprintf(stderr, _(" HTML character entities where possible (&eacute;), else numeric\tQ\n"));
  else {
    fprintf(stderr, _(" HTML character entities\tQ\n"));
  }
  if(!ToAsciiP) fprintf(stderr,
	       _(" all three types of HTML  escape:  hexadecimal  character references,\n\tdecimal character references, and character entities\tY\n"));
}
