/* Time-stamp: <2011-05-14 19:03:13 poser>
 *
 * Converts UTF-8 Unicode to pure 7-bit ASCII using any of a number
 * of different representations. 
 * 
 * Copyright (C) 2004-2011 William J. Poser (billposer@alum.mit.edu)
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

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif
#ifdef HAVE_LIBINTL_H
#include <libintl.h>
#define _(String) gettext(String)
#else
#define _(x) (x)
#endif
#include "unicode.h"
#include "enttbl.h"
#include "u2a_endian.h"
#include "utf8error.h"
#include "exitcode.h"
#include "formats.h"

#if defined(__DATE__) && defined(__TIME__)
#define HAVE_DATE_TIME
char compdate[]=  __DATE__ " " __TIME__ ;
#else
char compdate[]= "";
#endif
#define MSGSIZE 128

char version[]=PACKAGE_VERSION;
char pgname[]="uni2ascii";

#ifndef LOCALEDIR 
#define LOCALEDIR "/usr/local/share/locale"
#endif

char msg [MSGSIZE];

#include <sys/utsname.h>

#ifdef HAVE_GNU_LIBC_VERSION_H
#include <gnu/libc-version.h>
#endif

void
ShowVersion(FILE *fp)   
{
  extern char version[];
  char *vp;
  char vnum[11+1];
  struct utsname utsbuf;

  fprintf(fp,"%s %s\n",pgname,version);
#ifdef HAVE_GNU_LIBC_VERSION_H
  fprintf(fp,_("  glibc        %s\n"),gnu_get_libc_version());
#endif

  if (uname(&utsbuf) >= 0) {
    fprintf(fp,_("Compiled %s on %s\nunder %s %s %s\n"),
	    compdate,
	    utsbuf.machine,
	    utsbuf.sysname,
	    utsbuf.release,
	    utsbuf.version);
  }
  else fprintf(fp,_("Compiled %s\n"),compdate);
}


void Copyright (void) {
  fprintf(stderr,"Copyright (C) 2004-2011 William J. Poser\n");
  fprintf(stderr,"This program is free software; you can redistribute\n\
it and/or modify it under the terms of version 3 of\n\
the GNU General Public License as published by the\n\
Free Software Foundation.\n");
  fprintf(stderr,"Report bugs to: billposer@alum.mit.edu.\n");
}

void
ShowUsage(void){
  fprintf(stderr,_("This program is a filter which converts UTF-8 Unicode\n\
to any of a variety 7-bit ASCII textual representations.\n"));
  fprintf(stderr,_("By default all characters above 0x7F are converted to the specified\n\
format except for newline and the space characters (space, tab\n\
ideographic space, ethiopic word space and ogham space mark).\n\
Options allow conversion of the ASCII characters as well\n\
and for conversion even of newline and space characters.\n"));
  fprintf(stderr,_("Usage: %s [flags] (<input file name>)\n"),pgname);
  fprintf(stderr,_("       -h Print this usage message.\n"));
  fprintf(stderr,_("       -v Print version information.\n"));
  fprintf(stderr,_("       -L List format specifications.\n"));
  fprintf(stderr,_("       -E List the expansions performed by the -x option.\n"));
  fprintf(stderr,_("       -q Quiet. Do not chat unnecessarily while working.\n"));
  fprintf(stderr,_("       -a <format specification>.\n"));
  fprintf(stderr,_("       -c Convert circled and parenthesized characters.\n"));
  fprintf(stderr,_("       -e Convert to approximate ASCII equivalents.\n"));
  fprintf(stderr,_("       -f Convert formal equivalents to ASCII.\n"));
  fprintf(stderr,_("       -x Expand certain single characters to equivalent sequences.\n"));
  fprintf(stderr,_("       -y Convert certain characters to approximate single-characters.\n"));
  fprintf(stderr,_("       -d Strip diacritics.\n"));
  fprintf(stderr,_("       -S <Unicode:ASCII>  define a custom substitution\n"));
  fprintf(stderr,_("       -n Convert newlines.\n"));
  fprintf(stderr,_("       -s Convert space characters.\n"));
  fprintf(stderr,_("       -P Pass through Unicode if not transformed - do not ascify\n"));
  fprintf(stderr,_("       -p Convert codepoints below 0x80 except for 0x0A and 0x20 too.\n"));
  fprintf(stderr,_("       -l Use lower-case a-f when generating hex.\n"));
  fprintf(stderr,_("       -w Add a space after each converted item.\n"));
  fprintf(stderr,_("       -B Best ASCII. Transform to ASCII if possible. Combines cdefx.\n"));
  fprintf(stderr,
	  _("       -Z <format> Use the specified format\n"));
  fprintf(stderr,_("Report bugs to: billposer@alum.mit.edu\n"));
}

void ListExpansions(void){
  fprintf(stderr,_("The expansions performed by the -x flag are:\n"));
  fprintf(stderr,_("          U+00A2 CENT SIGN                   -> cent\n"));
  fprintf(stderr,_("          U+00A3 POUND SIGN                  -> pound\n"));
  fprintf(stderr,_("          U+00A5 YEN SIGN                    -> yen\n"));
  fprintf(stderr,_("          U+00A9 COPYRIGHT SYMBOL            -> (c)\n"));
  fprintf(stderr,_("          U+00AE REGISTERED SYMBOL           -> (R)\n"));
  fprintf(stderr,_("          U+00BC ONE QUARTER                 -> 1/4\n"));
  fprintf(stderr,_("          U+00BD ONE HALF                    -> 1/2\n"));
  fprintf(stderr,_("          U+00BE THREE QUARTERS              -> 3/4\n"));
  fprintf(stderr,_("          U+00C6 CAPITAL LETTER ASH          -> AE\n"));
  fprintf(stderr,_("          U+00DF SMALL LETTER SHARP S        -> ss\n"));
  fprintf(stderr,_("          U+00E6 SMALL LETTER ASH            -> ae\n"));
  fprintf(stderr,_("          U+0132 LIGATURE IJ                 -> IJ\n"));
  fprintf(stderr,_("          U+0133 LIGATURE ij                 -> ij\n"));
  fprintf(stderr,_("          U+0152 LIGATURE OE                 -> OE\n"));
  fprintf(stderr,_("          U+0153 LIGATURE oe                 -> oe\n"));
  fprintf(stderr,_("          U+01F1 CAPITAL LETTER DZ           -> DZ\n"));
  fprintf(stderr,_("          U+01F2 MIXED LETTER Dz             -> Dz\n"));
  fprintf(stderr,_("          U+01F3 SMALL LETTER dz             -> dz\n"));
  fprintf(stderr,_("          U+02A6 SMALL LETTER TS DIGRAPH     -> ts\n"));
  fprintf(stderr,_("          U+2026 HORIZONTAL ELLIPSIS         -> ...\n"));
  fprintf(stderr,_("          U+20AC EURO SIGN                   -> euro\n"));
  fprintf(stderr,_("          U+2190 LEFTWARDS ARROW             -> <-\n"));
  fprintf(stderr,_("          U+2192 RIGHTWARDS ARROW            -> ->\n"));
  fprintf(stderr,_("          U+21D0 LEFTWARDS DOUBLE ARROW      -> <=\n"));
  fprintf(stderr,_("          U+21D2 RIGHTWARDS DOUBLE ARROW     -> =>\n"));
  fprintf(stderr,_("          U+22EF MIDLINE HORIZONTAL ELLIPSIS -> ...\n"));
  fprintf(stderr,_("          U+FB00 LATIN SMALL LIGATURE FF -> ff\n"));
  fprintf(stderr,_("          U+FB01 LATIN SMALL LIGATURE FI -> fi\n"));
  fprintf(stderr,_("          U+FB02 LATIN SMALL LIGATURE FL -> fl\n"));
  fprintf(stderr,_("          U+FB03 LATIN SMALL LIGATURE FFI -> ffi\n"));
  fprintf(stderr,_("          U+FB04 LATIN SMALL LIGATURE FFL -> ffl\n"));
  fprintf(stderr,_("          U+FB06 LATIN SMALL LIGATURE ST -> st\n"));
}

void ListSingleApproximations(void){
  fprintf(stderr,_("The approximations produced by the -y flag are:\n"));
  fprintf(stderr,_("          U+0085 NEXT LINE                   -> newline\n"));
  fprintf(stderr,_("          U+00A2 CENT SIGN                   -> C\n"));
  fprintf(stderr,_("          U+00A3 POUND SIGN                  -> #\n"));
  fprintf(stderr,_("          U+00A5 YEN SIGN                    -> Y\n"));
  fprintf(stderr,_("          U+00A9 COPYRIGHT SYMBOL            -> C\n"));
  fprintf(stderr,_("          U+00AE REGISTERED SYMBOL           -> R\n"));
  fprintf(stderr,_("          U+00AF MACRON                      -> -\n"));
  fprintf(stderr,_("          U+00BC ONE QUARTER                 -> -\n"));
  fprintf(stderr,_("          U+00BD ONE HALF                    -> -\n"));
  fprintf(stderr,_("          U+00BE THREE QUARTERS              -> -\n"));
  fprintf(stderr,_("          U+00B7 MIDDLE DOT                  -> .\n"));
  fprintf(stderr,_("          U+00C6 CAPITAL LETTER ASH          -> A\n"));
  fprintf(stderr,_("          U+00DF SMALL LETTER SHARP S        -> s\n"));
  fprintf(stderr,_("          U+00E6 SMALL LETTER ASH            -> a\n"));
  fprintf(stderr,_("          U+0132 LIGATURE IJ                 -> I\n"));
  fprintf(stderr,_("          U+0133 LIGATURE ij                 -> i\n"));
  fprintf(stderr,_("          U+0152 LIGATURE OE                 -> O\n"));
  fprintf(stderr,_("          U+0153 LIGATURE oe                 -> o\n"));
  fprintf(stderr,_("          U+01F1 CAPITAL LETTER DZ           -> D\n"));
  fprintf(stderr,_("          U+01F2 MIXED LETTER Dz             -> D\n"));
  fprintf(stderr,_("          U+01F3 SMALL LETTER dz             -> d\n"));
  fprintf(stderr,_("          U+02A6 SMALL LETTER TS DIGRAPH     -> t\n"));
  fprintf(stderr,_("          U+2028 LINE SEPARATOR              -> newline\n"));
  fprintf(stderr,_("          U+2026 HORIZONTAL ELLIPSIS         -> .\n"));
  fprintf(stderr,_("          U+20AC EURO SIGN                   -> E\n"));
  fprintf(stderr,_("          U+2190 LEFTWARDS ARROW             -> <\n"));
  fprintf(stderr,_("          U+2192 RIGHTWARDS ARROW            -> >\n"));
  fprintf(stderr,_("          U+21D0 LEFTWARDS DOUBLE ARROW      -> <\n"));
  fprintf(stderr,_("          U+21D2 RIGHTWARDS DOUBLE ARROW     -> >\n"));
  fprintf(stderr,_("          U+2216 SET MINUS                   -> \\\n"));
  fprintf(stderr,_("          U+2217 ASTERISK OPERATOR           -> *\n"));
  fprintf(stderr,_("          U+2223 DIVIDES                     -> |\n"));
  fprintf(stderr,_("          U+22EF MIDLINE HORIZONTAL ELLIPSIS -> .\n"));
}

/* Swap the byte order of a 4 byte integer */

void
lswab(unsigned long *val)
{
  union{
    long l;
    char c[4];
  } u;

  unsigned char tmp;

  u.l = *val;

  /* Swap peripheral bytes */
  tmp = u.c[0];
  u.c[0] = u.c[3];
  u.c[3] = tmp;
  /* Swap medial bytes */
  tmp = u.c[1];
  u.c[1] = u.c[2];
  u.c[2] = tmp;

  *val = u.l;
}

static char *Formats [] = {
  "&#x%04x;",  /* HTMLX */
  "&#x%04X;",
  "&#%04d;",    /* HTMLD */
  "&#%04d;",
  "\\#x%04x;",  /* SGMLX */
  "\\#x%04X;",
  "\\#%04d;",    /* SGMLD */
  "\\#%04d;",
  "\\u%04x",   /* BSLU */
  "\\u%04X",   
  "\\x%04x",    /* BSLX */
  "\\x%04X",
  "0x%04x",    /* STDX */
  "0x%04X",   
  "#x%04x",    /* CLSX */
  "#x%04X",   
  "%04x",       /* RAWX */
  "%04X",   
  "\\x{%04x}",	/* BSLXB */
  "\\x{%04X}",
  "<U%04x>", 	/* ABUX */
  "<U%04X>",
  "U%04x", 	/* JUUX */
  "U%04X", 
  "u%04x", 	/* JuUX */
  "u%04X",
  "U+%04x",	/* UPLX */
  "U+%04X",
  "X\'%04x\'",	/* XQ */
  "X\'%04X\'",
  "\\u%05d",	/* BSLUD */
  "\\u%05d",
  "v%05d",	/* PERLV */
  "v%05d",
  "$%04x",  	/* DOLLAR */
  "$%04X",
  "16#%04x",	/* PSPT */
  "16#%04X",
  "#16r%04x",	/* CLR */
  "#16r%04X",
  "16#%04x#",	/* ADA */
  "16#%04X#",
  "&#%02x;",   /* HDML */ 
  "&#%02X;",
  "\\%03hho\\%03hho\\%03hho",	/* BYTEO */
  "\\%03hho\\%03hho\\%03hho",
  "\\d%03hhd\\d%03hhd\\d%03hhd",	/* BYTED */
  "\\d%03hhd\\d%03hhd\\d%03hhd",
  "\\x%02hhx\\x%02hhx\\x%02hhx",/* BYTEH */
  "\\x%02hhX\\x%02hhX\\x%02hhX",
  "_x%04x_", /* OOXML */
  "_x%04X_",
  "%%u%04x", 	/* PCTUX */
  "%%u%04X"
  };

/*  Returns 1 if it handled the character, 0 if not. */
/* Done for letters, probably should add numbers */
int AscifyEnclosed(UTF32 c) {
  switch (c)
    {
      /* Parenthesized letters */
    case 0x249C:
    case 0x24D0:
      putchar(0x61);
      break;
    case 0x249D:
    case 0x24D1:
      putchar(0x62);
      break;
    case 0x249E:
    case 0x24D2:
      putchar(0x63);
      break;
    case 0x249F:
    case 0x24D3:
      putchar(0x64);
      break;
    case 0x24A0:
    case 0x24D4:
      putchar(0x65);
      break;
    case 0x24A1:
    case 0x24D5:
      putchar(0x66);
      break;
    case 0x24A2:
    case 0x24D6:
      putchar(0x67);
      break;
    case 0x24A3:
    case 0x24D7:
      putchar(0x68);
      break;
    case 0x24A4:
    case 0x24D8:
      putchar(0x69);
      break;
    case 0x24A5:
    case 0x24D9:
      putchar(0x6A);
      break;
    case 0x24A6:
    case 0x24DA:
      putchar(0x6B);
      break;
    case 0x24A7:
    case 0x24DB:
      putchar(0x6C);
      break;
    case 0x24A8:
    case 0x24DC:
      putchar(0x6D);
      break;
    case 0x24A9:
    case 0x24DD:
      putchar(0x6E);
      break;
    case 0x24AA:
    case 0x24DE:
      putchar(0x6F);
      break;
    case 0x24AB:
    case 0x24DF:
      putchar(0x70);
      break;
    case 0x24AC:
    case 0x24E0:
      putchar(0x71);
      break;
    case 0x24AD:
    case 0x24E1:
      putchar(0x72);
      break;
    case 0x24AE:
    case 0x24E2:
      putchar(0x73);
      break;
    case 0x24AF:
    case 0x24E3:
      putchar(0x74);
      break;
    case 0x24B0:
    case 0x24E4:
      putchar(0x75);
      break;
    case 0x24B1:
    case 0x24E5:
      putchar(0x76);
      break;
    case 0x24B2:
    case 0x24E6:
      putchar(0x77);
      break;
    case 0x24B3:
    case 0x24E7:
      putchar(0x78);
      break;
    case 0x24B4:
    case 0x24E8:
      putchar(0x79);
      break;
    case 0x24B5:
    case 0x24E9:
      putchar(0x7A);
      break;
      /* Circled capital letters */
    case 0x24B6:
      putchar(0x41);
      break;
    case 0x24B7:
      putchar(0x42);
      break;
    case 0x24B8:
      putchar(0x43);
      break;
    case 0x24B9:
      putchar(0x44);
      break;
    case 0x24BA:
      putchar(0x45);
      break;
    case 0x24BB:
      putchar(0x46);
      break;
    case 0x24BC:
      putchar(0x47);
      break;
    case 0x24BD:
      putchar(0x48);
      break;
    case 0x24BE:
      putchar(0x49);
      break;
    case 0x24BF:
      putchar(0x4A);
      break;
    case 0x24C0:
      putchar(0x4B);
      break;
    case 0x24C1:
      putchar(0x4C);
      break;
    case 0x24C2:
      putchar(0x4D);
      break;
    case 0x24C3:
      putchar(0x4E);
      break;
    case 0x24C4:
      putchar(0x4F);
      break;
    case 0x24C5:
      putchar(0x50);
      break;
    case 0x24C6:
      putchar(0x51);
      break;
    case 0x24C7:
      putchar(0x52);
      break;
    case 0x24C8:
      putchar(0x53);
      break;
    case 0x24C9:
      putchar(0x54);
      break;
    case 0x24CA:
      putchar(0x55);
      break;
    case 0x24CB:
      putchar(0x56);
      break;
    case 0x24CC:
      putchar(0x57);
      break;
    case 0x24CD:
      putchar(0x58);
      break;
    case 0x24CE:
      putchar(0x59);
      break;
    case 0x24CF:
      putchar(0x5A);
      break;
    default:
      return 0;
    }
  return 1;
}

/*  Returns 1 if it replaced the character, 0 if not. */
int AscifyEquiv(UTF32 c) {
  int retval = 1;
  switch (c)
    {
    case 0x0085:		/* next line */
    case 0x2028:		/* line separator */ 
      putchar('\n');
      break;
    case 0x00AF:
      putchar('-');		/* macron */
      break;
    case 0x00B7:		/* middle dot */
      putchar('.');
      break;
    case 0xFE60:
      putchar(0x26);		/* ampersand */
      break;
    case 0x204E:
    case 0x2217:
    case 0x2731:
    case 0xFE61:
      putchar(0x2A);		/* asterisk */
      break;
    case 0xFE62:		/* plus sign */
      putchar(0x2B);
      break;
    case 0x00AD:
    case 0x2010:
    case 0x2011:
    case 0x2012:
    case 0x2013:
    case 0x2014:
    case 0x2212:
    case 0x2500:
    case 0x2501:
      putchar(0x2D);		/* Hyphen */
      break;
    case 0x2502:
    case 0x2503:
      putchar(0x7C);		/* Vertical line */
      break;
    case 0x00AB:
    case 0x00BB:
    case 0x201C:
    case 0x201D:
    case 0x201E:
    case 0x201F:
    case 0x2033:
    case 0x275D:
    case 0x275E:
      putchar(0x22);		/* Double quote */
      break;
    case 0x2018:
    case 0x201A:
    case 0x201B:
    case 0x2039:
      putchar(0x60);		/* left single quote */
      break;
    case 0x2022:		/* Bullet */
      putchar('o');
      break;
    case 0x2019:
    case 0x203A:
      putchar(0x27);		/* right or neutral single quote */
      break;
    case 0x222A:		/* Union */
      putchar('U');
      break;
    case 0x00A0:		/* no break space */
    case 0x1361:		/* ethiopic word space */
    case 0x1680:		/* ogham space */
    case 0x2000:		/* en quad */
    case 0x2001:		/* em quad */
    case 0x2002:		/* en space */
    case 0x2003:		/* em space */
    case 0x2004:		/* three-per-em space */
    case 0x2005:		/* four-per-em space */
    case 0x2006:		/* six-per-em space */
    case 0x2007:		/* figure space */
    case 0x2008:		/* punctuation space */
    case 0x2009:		/* thin space */
    case 0x200A:		/* hair space */
    case 0x200B:		/* zero-width space */
    case 0x3000:		/* ideographic space */
      putchar(0x20);		/* Space */
      break;
    case 0x2215:		/* division slash */
      putchar(0x2F);
      break;
    case 0x2216:		/* set minus */
      putchar(0x5C);
      break;
    case 0x2223:		/* divides */
      putchar('|');
      break;
    default:
      retval = 0;
    }
  return retval;
}

/*  Returns 1 if it handled the character, 0 if not. */
int AscifyDiacritics(UTF32 c) {
  switch (c)
    {
    case 0x226E:
      putchar(0x3C);
      break;
    case 0x226F:
      putchar(0x3E);
      break;
    case 0x00C0:
    case 0x00C1:
    case 0x00C2:
    case 0x00C3:
    case 0x00C4:
    case 0x00C5:
    case 0x0100:
    case 0x0102:
    case 0x0104:
    case 0x01CD:
    case 0x01DE:
    case 0x01E0:
    case 0x01E2:
    case 0x01FA:
    case 0x0200:
    case 0x0202:
    case 0x0226:
    case 0x1D00:
    case 0x1E00:
    case 0x1EA0:
    case 0x1EA2:
    case 0x1EA4:
    case 0x1EA6:
    case 0x1EA8:
    case 0x1EAA:
    case 0x1EAC:
    case 0x1EAE:
    case 0x1EB0:
    case 0x1EB2:
    case 0x1EB4:
    case 0x1EB6:
      putchar(0x41);		/* A */
      break;
    case 0x0181:
    case 0x0182:
    case 0x0299:
    case 0x1D03:
    case 0x1E02:
    case 0x1E04:
    case 0x1E06:
      putchar(0x42);		/* B */
      break;
    case 0x00C7:
    case 0x0106:
    case 0x0108:
    case 0x010A:
    case 0x010C:
    case 0x0187:
    case 0x023B:
    case 0x1E08:
      putchar(0x43);		/* C */
      break;
    case 0x010E:
    case 0x0110:
    case 0x018A:
    case 0x018B:
    case 0x1E0A:
    case 0x1E0C:
    case 0x1E0E:
    case 0x1E10:
    case 0x1E12:
      putchar(0x44);		/* D */
      break;
    case 0x00C8:
    case 0x00C9:
    case 0x00CA:
    case 0x00CB:
    case 0x0112:
    case 0x0114:
    case 0x0116:
    case 0x0118:
    case 0x011A:
    case 0x0204:
    case 0x0206:
    case 0x0228:
    case 0x1D07:
    case 0x1E14:
    case 0x1E16:
    case 0x1E18:
    case 0x1E1A:
    case 0x1E1C:
    case 0x1EB8:
    case 0x1EBA:
    case 0x1EBC:
    case 0x1EBE:
    case 0x1EC0:
    case 0x1EC2:
    case 0x1EC4:
    case 0x1EC6:
      putchar(0x45);		/* E */
      break;
    case 0x0191:
    case 0x1E1E:
      putchar(0x46);		/* F */
      break;
    case 0x011C:
    case 0x011E:
    case 0x0120:
    case 0x0122:
    case 0x0193:
    case 0x01E4:
    case 0x01E6:
    case 0x01F4:
    case 0x1E20:
      putchar(0x47);		/* G */
      break;
    case 0x0124:
    case 0x0126:
    case 0x021E:
    case 0x1E22:
    case 0x1E24:
    case 0x1E26:
    case 0x1E28:
    case 0x1E2A:
      putchar(0x48);		/* H */
      break;
    case 0x00CC:
    case 0x00CD:
    case 0x00CE:
    case 0x00CF:
    case 0x0128:
    case 0x012A:
    case 0x012C:
    case 0x012E:
    case 0x0130:
    case 0x0197:
    case 0x01CF:
    case 0x0208:
    case 0x020A:
    case 0x026A:
    case 0x1D7B:
    case 0x1E2C:
    case 0x1E2E:
    case 0x1EC8:
    case 0x1ECA:
      putchar(0x49);		/* I */
      break;
    case 0x0134:
      putchar(0x4A);		/* J */
      break;
    case 0x0136:
    case 0x0198:
    case 0x01E8:
    case 0x1E30:
    case 0x1E32:
    case 0x1E34:
      putchar(0x4B);		/* K */
      break;
    case 0x0139:
    case 0x013B:
    case 0x013D:
    case 0x023D:
    case 0x1E36:
    case 0x1E38:
    case 0x1E3A:
    case 0x1E3C:
      putchar(0x4C);		/* L */
      break;
    case 0x1E3E:
    case 0x1E40:
    case 0x1E42:
      putchar(0x4D);		/* M */
      break;
    case 0x00D1:
    case 0x0143:
    case 0x0145:
    case 0x0147:
    case 0x019D:
    case 0x01F8:
    case 0x0220:
    case 0x1E44:
    case 0x1E46:
    case 0x1E48:
    case 0x1E4A:
      putchar(0x4E);		/* N */
      break;
    case 0x00D2:
    case 0x00D3:
    case 0x00D4:
    case 0x00D5:
    case 0x00D6:
    case 0x00D8:
    case 0x014C:
    case 0x014E:
    case 0x0150:
    case 0x019F:
    case 0x01A0:
    case 0x01D1:
    case 0x01EA:
    case 0x01EC:
    case 0x01FE:
    case 0x020C:
    case 0x020E:
    case 0x022A:
    case 0x022C:
    case 0x022E:
    case 0x0230:
    case 0x1D0F:
    case 0x1E4C:
    case 0x1E4E:
    case 0x1E50:
    case 0x1E52:
    case 0x1ECC:
    case 0x1ECE:
    case 0x1ED0:
    case 0x1ED2:
    case 0x1ED4:
    case 0x1ED6:
    case 0x1ED8:
    case 0x1EDA:
    case 0x1EDC:
    case 0x1EDE:
    case 0x1EE0:
    case 0x1EE2:
      putchar(0x4F);		/* O */
      break;
    case 0x01A4:
    case 0x1E54:
    case 0x1E56:
      putchar(0x50);		/* P */
      break;
    case 0x0154:
    case 0x0156:
    case 0x0158:
    case 0x0210:
    case 0x0212:
    case 0x1E58:
    case 0x1E5A:
    case 0x1E5C:
    case 0x1E5E:
      putchar(0x52);		/* R */
      break;
    case 0x015A:
    case 0x015C:
    case 0x015E:
    case 0x0160:
    case 0x0218:
    case 0x1E60:
    case 0x1E62:
    case 0x1E64:
    case 0x1E66:
    case 0x1E68:
      putchar(0x53);		/* S */
      break;
    case 0x0162:
    case 0x0164:
    case 0x01AC:
    case 0x01AE:
    case 0x021A:
    case 0x023E:
    case 0x1E6A:
    case 0x1E6C:
    case 0x1E6E:
    case 0x1E70:
      putchar(0x54);		/* T */
      break;
    case 0x00D9:
    case 0x00DA:
    case 0x00DB:
    case 0x00DC:
    case 0x0168:
    case 0x016A:
    case 0x016C:
    case 0x016E:
    case 0x0170:
    case 0x0172:
    case 0x01AF:
    case 0x01D3:
    case 0x01D5:
    case 0x01D7:
    case 0x01D9:
    case 0x01DB:
    case 0x0214:
    case 0x0216:
    case 0x1D1C:
    case 0x1D7E:
    case 0x1E72:
    case 0x1E74:
    case 0x1E76:
    case 0x1E78:
    case 0x1E7A:
    case 0x1EE4:
    case 0x1EE6:
    case 0x1EE8:
    case 0x1EEA:
    case 0x1EEC:
    case 0x1EEE:
    case 0x1EF0:
      putchar(0x55);		/* U */
      break;
    case 0x01B2:
    case 0x1E7C:
    case 0x1E7E:
      putchar(0x56);		/* V */
      break;
    case 0x0174:
    case 0x1E80:
    case 0x1E82:
    case 0x1E84:
    case 0x1E86:
    case 0x1E88:
      putchar(0x57);		/* W */
      break;
    case 0x1E8A:
    case 0x1E8C:
      putchar(0x58);		/* X */
      break;
    case 0x00DD:
    case 0x0176:
    case 0x0178:
    case 0x01B3:
    case 0x0232:
    case 0x028F:
    case 0x1E8E:
    case 0x1EF2:
    case 0x1EF4:
    case 0x1EF6:
    case 0x1EF8:
      putchar(0x59);		/* Y */
      break;
    case 0x0179:
    case 0x017B:
    case 0x017D:
    case 0x01B5:
    case 0x0224:
    case 0x1D22:
    case 0x1E90:
    case 0x1E92:
    case 0x1E94:
      putchar(0x5A);		/* Z */
      break;
    case 0x00E0: 		/* a */
    case 0x00E1:
    case 0x00E2:
    case 0x00E3:
    case 0x00E4:
    case 0x00E5:
    case 0x0101:
    case 0x0103:
    case 0x0105:
    case 0x01CE:
    case 0x01DF:
    case 0x01E1:
    case 0x01FB:
    case 0x0201:
    case 0x0203:
    case 0x0227:
    case 0x1E01:
    case 0x1E9A:
    case 0x1EA1:
    case 0x1EA3:
    case 0x1EA5:
    case 0x1EA7:
    case 0x1EA9:
    case 0x1EB1:
    case 0x1EB3:
    case 0x1EB5:
    case 0x1EB7:
      putchar(0x61);
      break;
    case 0x0180:		/* b */
    case 0x0183:
    case 0x0253:
    case 0x1E03:
    case 0x1E05:
    case 0x1E07:
      putchar(0x62);
      break;
    case 0x00E7:
    case 0x0107:
    case 0x0109:
    case 0x010B:
    case 0x010D:
    case 0x0188:
    case 0x0255:
    case 0x1E09:
      putchar(0x63);		/* c */
      break;
    case 0x010F:
    case 0x0111:
    case 0x018C:
    case 0x0221:
    case 0x0256:
    case 0x0257:
    case 0x1D6D:
    case 0x1D81:
    case 0x1D91:
    case 0x1E0B:
    case 0x1E0D:
    case 0x1E0F:
    case 0x1E11:
    case 0x1E13:
      putchar(0x64);		/* d */
      break;
    case 0x00E8:
    case 0x00E9:
    case 0x00EA:
    case 0x00EB:
    case 0x0113:
    case 0x0115:
    case 0x0117:
    case 0x0119:
    case 0x011B:
    case 0x0205:
    case 0x0207:
    case 0x0229:
    case 0x1E15:
    case 0x1E17:
    case 0x1E19:
    case 0x1E1B:
    case 0x1E1D:
    case 0x1EB9:
    case 0x1EBB:
    case 0x1EBD:
    case 0x1EBF:
    case 0x1EC1:
    case 0x1EC3:
    case 0x1EC5:
    case 0x1EC7:
      putchar(0x65);		/* e */
      break;
    case 0x0192:
    case 0x1D6E:
    case 0x1D82:
    case 0x1E1F:
      putchar(0x66);		/* f */
      break;
    case 0x011D:
    case 0x011F:
    case 0x0121:
    case 0x0123:
    case 0x01E5:
    case 0x01E7:
    case 0x01F5:
    case 0x0260:
    case 0x1E21:
      putchar(0x67);		/* g */
      break;
    case 0x0125:
    case 0x0127:
    case 0x021F:
    case 0x0266:
    case 0x1E23:
    case 0x1E25:
    case 0x1E27:
    case 0x1E29:
    case 0x1E2B:
    case 0x1E96:
      putchar(0x68);		/* h */
      break;
    case 0x00EC:
    case 0x00ED:
    case 0x00EE:
    case 0x00EF:
    case 0x0129:
    case 0x012B:
    case 0x012D:
    case 0x012F:
    case 0x01D0:
    case 0x0209:
    case 0x020B:
    case 0x0268:
    case 0x1D96:
    case 0x1E2D:
    case 0x1E2F:
    case 0x1EC9:
    case 0x1ECB:
      putchar(0x69);		/* i */
      break;
    case 0x0135:
    case 0x01F0:
    case 0x029D:
      putchar(0x6A);		/* j */
      break;
    case 0x0137:
    case 0x0199:
    case 0x01E9:
    case 0x1D84:
    case 0x1E31:
    case 0x1E33:
    case 0x1E35:
      putchar(0x6B);		/* k */
      break;
    case 0x013A:
    case 0x013C:
    case 0x013E:
    case 0x019A:
    case 0x0234:
    case 0x026B:
    case 0x026C:
    case 0x026D:
    case 0x1D85:
    case 0x1E37:
    case 0x1E39:
    case 0x1E3B:
    case 0x1E3D:
      putchar(0x6C);		/* l */
      break;
    case 0x0271:
    case 0x1D6F:
    case 0x1D86:
    case 0x1E3F:
    case 0x1E41:
    case 0x1E43:
      putchar(0x6D);		/* m */
      break;
    case 0x00F1:
    case 0x0144:
    case 0x0146:
    case 0x0148:
    case 0x019E:
    case 0x01F9:
    case 0x0272:
    case 0x0273:
    case 0x1D70:
    case 0x1D87:
    case 0x1E45:
    case 0x1E47:
    case 0x1E49:
    case 0x1E4B:
      putchar(0x6E);		/* n */
      break;
    case 0x00F2:
    case 0x00F3:
    case 0x00F4:
    case 0x00F5:
    case 0x00F6:
    case 0x00F8:
    case 0x014D:
    case 0x014F:
    case 0x0151:
    case 0x01A1:
    case 0x01D2:
    case 0x01EB:
    case 0x01ED:
    case 0x01FF:
    case 0x020D:
    case 0x020F:
    case 0x022B:
    case 0x022D:
    case 0x022F:
    case 0x0231:
    case 0x1E4D:
    case 0x1E4F:
    case 0x1E51:
    case 0x1E53:
    case 0x1ECD:
    case 0x1ECF:
    case 0x1ED1:
    case 0x1ED3:
    case 0x1ED5:
    case 0x1ED7:
    case 0x1ED9:
    case 0x1EDB:
    case 0x1EDD:
    case 0x1EDF:
    case 0x1EE1:
    case 0x1EE3:
      putchar(0x6F);		/* o */
      break;
    case 0x01A5:
    case 0x1D71:
    case 0x1D7D:
    case 0x1D88:
    case 0x1E55:
    case 0x1E57:
      putchar(0x70);		/* p */
      break;
    case 0x02A0:
      putchar(0x71);		/* q */
      break;
    case 0x0155:
    case 0x0157:
    case 0x0159:
    case 0x0211:
    case 0x0213:
    case 0x027C:
    case 0x027D:
    case 0x027E:
    case 0x1D72:
    case 0x1D73:
    case 0x1D89:
    case 0x1E59:
    case 0x1E5B:
    case 0x1E5D:
    case 0x1E5F:
      putchar(0x72);		/* r */
      break;
    case 0x015B:
    case 0x015D:
    case 0x0219:
    case 0x023F:
    case 0x0282:
    case 0x0161:
    case 0x1D74:
    case 0x1D8A:
    case 0x1E61:
    case 0x1E63:
    case 0x1E65:
    case 0x1E67:
    case 0x1E69:
      putchar(0x73);		/* s */
      break;
    case 0x0165:
    case 0x01AB:
    case 0x01AD:
    case 0x021B:
    case 0x0288:
    case 0x1D75:
    case 0x1E6B:
    case 0x1E6D:
    case 0x1E6F:
    case 0x1E71:
    case 0x1E97:
      putchar(0x74);		/* t */
      break;
    case 0x00F9:
    case 0x00FA:
    case 0x00FB:
    case 0x00FC:
    case 0x0169:
    case 0x016D:
    case 0x016F:
    case 0x0171:
    case 0x0173:
    case 0x01B0:
    case 0x01D4:
    case 0x01D6:
    case 0x01D8:
    case 0x01DA:
    case 0x01DC:
    case 0x0215:
    case 0x0217:
    case 0x0289:
    case 0x1D99:
    case 0x1E73:
    case 0x1E75:
    case 0x1E77:
    case 0x1E79:
    case 0x1E7B:
    case 0x1EE5:
    case 0x1EE7:
    case 0x1EE9:
    case 0x1EEB:
    case 0x1EED:
    case 0x1EEF:
    case 0x1EF1:
      putchar(0x75);		/* u */
      break;
    case 0x028B:
    case 0x1D8C:
    case 0x1E7D:
    case 0x1E7F:
      putchar(0x76);		/* v */
      break;
    case 0x0175:
    case 0x1E81:
    case 0x1E83:
    case 0x1E85:
    case 0x1E87:
    case 0x1E89:
    case 0x1E98:
      putchar(0x77);		/* w */
      break;
    case 0x1D8D:
    case 0x1E8B:
    case 0x1E8D:
      putchar(0x78);		/* x */
      break;
    case 0x00FD:
    case 0x00FE:
    case 0x00FF:
    case 0x0177:
    case 0x01B4:
    case 0x0233:
    case 0x1E8F:
    case 0x1E99:
    case 0x1EF3:
    case 0x1EF5:
    case 0x1EF7:
    case 0x1EF9:
      putchar(0x79);		/* y */
      break;
    case 0x017A:
    case 0x017C:
    case 0x017E:
    case 0x01B6:
    case 0x0225:
    case 0x0290:
    case 0x0291:
    case 0x1E91:
    case 0x1E93:
    case 0x1E95:
      putchar(0x7A);		/* z */
      break;
    case 0x0300:		/* Combining diacrtics - just skip */
    case 0x0301:
    case 0x0302:
    case 0x0303:
    case 0x0304:
    case 0x0305:
    case 0x0306:
    case 0x0307:
    case 0x0308:
    case 0x0309:
    case 0x030A:
    case 0x030B:
    case 0x030C:
    case 0x030D:
    case 0x030E:
    case 0x030F:
    case 0x0310:
    case 0x0311:
    case 0x0312:
    case 0x0313:
    case 0x0314:
    case 0x0315:
    case 0x0316:
    case 0x0317:
    case 0x0318:
    case 0x0319:
    case 0x031A:
    case 0x031B:
    case 0x031C:
    case 0x031D:
    case 0x031E:
    case 0x031F:
    case 0x0320:
    case 0x0321:
    case 0x0322:
    case 0x0323:
    case 0x0324:
    case 0x0325:
    case 0x0326:
    case 0x0327:
    case 0x0328:
    case 0x0329:
    case 0x032A:
    case 0x032B:
    case 0x032C:
    case 0x032D:
    case 0x032E:
    case 0x032F:
    case 0x0330:
    case 0x0331:
    case 0x0332:
    case 0x0333:
    case 0x0334:
    case 0x0335:
    case 0x0336:
    case 0x0337:
    case 0x0338:
    case 0x0339:
    case 0x033A:
    case 0x033B:
    case 0x033C:
    case 0x033D:
    case 0x033E:
    case 0x033F:
    case 0x0340:
    case 0x0341:
    case 0x0342:
    case 0x0343:
    case 0x0344:
    case 0x0345:
    case 0x0346:
    case 0x0347:
    case 0x0348:
    case 0x0349:
    case 0x034A:
    case 0x034B:
    case 0x034C:
    case 0x034D:
    case 0x034E:
    case 0x034F:
    case 0x0350:
    case 0x0351:
    case 0x0352:
    case 0x0353:
    case 0x0354:
    case 0x0355:
    case 0x0356:
    case 0x0357:
    case 0x0358:
    case 0x0359:
    case 0x035A:
    case 0x035B:
    case 0x035C:
    case 0x035D:
    case 0x035E:
    case 0x035F:
    case 0x0360:
    case 0x0361:
    case 0x0362:
      break;
    default:
      return 0;
    }
  return 1;
} /* End of AscifyDiacritics */

/* 
 * Convert stylistic variants to plain equivalent.
 *
 * Returns 1 if it handled the character, 0 if not.
 */
int AscifyStyle(UTF32 c) {
  switch (c)
    {				
    case 0x2070:
    case 0x2080:
    case 0xFF10:
      putchar(0x30); 		/* 0 */
      break;
    case 0x00B9:
    case 0x2081:
    case 0xFF11:
      putchar(0x31);		/* 1 */
      break;
    case 0x00B2:
    case 0x2082:
    case 0xFF12:
      putchar(0x32);
      break;
    case 0x00B3:
    case 0x2083:
    case 0xFF13:
      putchar(0x33);
      break;
    case 0x2074:
    case 0x2084:
    case 0xFF14:
      putchar(0x34);
      break;
    case 0x2075:
    case 0x2085:
    case 0xFF15:
      putchar(0x35);
      break;
    case 0x2076:
    case 0x2086:
    case 0xFF16:
      putchar(0x36);
      break;
    case 0x2077:
    case 0x2087:
    case 0xFF17:
      putchar(0x37);
      break;
    case 0x2078:
    case 0x2088:
    case 0xFF18:
      putchar(0x38);
      break;
    case 0x2079:
    case 0x2089:
    case 0xFF19:
      putchar(0x39);
      break;
    case 0xFE57:
    case 0xFF01:		/* Exclamation mark */
      putchar(0x21);
      break;
    case 0xFF02:		/* Quotation mark */
      putchar(0x22);
      break;
    case 0xFE5F:
    case 0xFF03:		/* Number sign */
      putchar(0x23);
      break;
    case 0xFE69:
    case 0xFF04:		/* Dollar sign */
      putchar(0x24);
      break;
    case 0xFE6A:
    case 0xFF05:		/* Percent sign */
      putchar(0x25);
      break;
    case 0xFE60:
    case 0xFF06:		/* Ampersand  */
      putchar(0x26);
      break;
    case 0xFF07:		/* Apostrophe */
      putchar(0x27);
      break;
    case 0x207D:
    case 0x208D:
    case 0xFE59:
    case 0xFF08:		/* Left parenthesis */
      putchar(0x28);
      break;
    case 0x207E:
    case 0x208E:
    case 0xFE5A:
    case 0xFF09:		/* Right parenthesis */
      putchar(0x29);
      break;
    case 0xFE61:
    case 0xFF0A:		/* Asterisk */
      putchar(0x2A);
      break;
    case 0x207A:
    case 0x208A:
    case 0xFE62:
    case 0xFF0B:		/* Plus sign */
      putchar(0x2B);
      break;
    case 0xFE50:
    case 0xFF0C:		/* Comma */
      putchar(0x2C);
      break;
    case 0x207B:
    case 0x208B:
    case 0xFE63:
    case 0xFF0D:		/* Hyphen */
      putchar(0x2D);
      break;
    case 0xFE52:
    case 0xFF0E:		/* Full stop */
      putchar(0x2E);
      break;
    case 0xFF0F:		/* Solidus */
      putchar(0x2F);
      break;
    case 0xFE55:
    case 0xFF1A:		/* Colon */
      putchar(0x3A);
      break;
    case 0xFE54:
    case 0xFF1B:		/* Semicolon */
      putchar(0x3B);
      break;
    case 0xFE64:
    case 0xFF1C:		/* Less than sign */
      putchar(0x3C);
      break;
    case 0x207C:
    case 0x208C:
    case 0xFE66:
    case 0xFF1D:		
      putchar(0x3D);		/* Equals sign */
      break;
    case 0xFE65:
    case 0xFF1E:		/* Greater than sign */
      putchar(0x3E);
      break;
    case 0xFE56:
    case 0xFF1F:		/* Question mark */
      putchar(0x3F);
      break;
    case 0xFE6B:
    case 0xFF20:		/* At sign */
      putchar(0x40);
      break;
    case 0xFF21:
    case 0x1D400:
    case 0x1D434:
    case 0x1D468:
    case 0x1D49C:
    case 0x1D4D0:
    case 0x1D504:
    case 0x1D538:
    case 0x1D56C:
    case 0x1D5A0:
    case 0x1D5D4:
    case 0x1D63C:
    case 0x1D670:
      putchar(0x41);
      break;
    case 0x212C:		/* B */
    case 0xFF22:
    case 0x1D401:
    case 0x1D435:
    case 0x1D469:
    case 0x1D4D1:
    case 0x1D505:
    case 0x1D539:
    case 0x1D56D:
    case 0x1D5A1:
    case 0x1D5D5:
    case 0x1D609:
    case 0x1D63D:
    case 0x1D671:
      putchar(0x42);
      break;
    case 0x2102:
    case 0x212D:
    case 0xFF23:
    case 0x1D402:
    case 0x1D436:
    case 0x1D46A:
    case 0x1D49E:
    case 0x1D4D2:
    case 0x1D56E:
    case 0x1D5A2:
    case 0x1D5D6:
    case 0x1D60A:
    case 0x1D63E:
    case 0x1D672:
      putchar(0x43);		/* C */
      break;
    case 0x2145:
    case 0xFF24:
    case 0x1D403:
    case 0x1D437:
    case 0x1D46B:
    case 0x1D49F:
    case 0x1D4D3:
    case 0x1D507:
    case 0x1D53B:
    case 0x1D56F:
    case 0x1D5A3:
    case 0x1D5D7:
    case 0x1D60B:
    case 0x1D63F:
    case 0x1D673:
      putchar(0x44);		/* D */
      break;
    case 0x2130:		/* E*/
    case 0xFF25:
    case 0x1D404:
    case 0x1D438:
    case 0x1D46C:
    case 0x1D4D4:
    case 0x1D508:
    case 0x1D53C:
    case 0x1D570:
    case 0x1D5A4:
    case 0x1D5D8:
    case 0x1D60C:
    case 0x1D640:
    case 0x1D674:
      putchar(0x45);
      break;
    case 0x2131:
    case 0x213F:
    case 0xFF26:
    case 0x1D405:
    case 0x1D439:
    case 0x1D46D:
    case 0x1D4D5:
    case 0x1D509:
    case 0x1D53D:
    case 0x1D571:
    case 0x1D5A5:
    case 0x1D5D9:
    case 0x1D60D:
    case 0x1D641:
    case 0x1D675:
      putchar(0x46);		/* F */
      break;
    case 0x0262:
    case 0xFF27:
    case 0x1D406:
    case 0x1D43A:
    case 0x1D46E:
    case 0x1D4A2:
    case 0x1D4D6:
    case 0x1D50A:
    case 0x1D53E:
    case 0x1D572:
    case 0x1D5A6:
    case 0x1D5DA:
    case 0x1D60E:
    case 0x1D642:
    case 0x1D676:
      putchar(0x47);		/* G */
      break;
    case 0x029C:
    case 0x210B:
    case 0x210C:
    case 0x210D:
    case 0xFF28:
    case 0x1D407:
    case 0x1D43B:
    case 0x1D46F:
    case 0x1D4D7:
    case 0x1D573:
    case 0x1D5A7:
    case 0x1D5DB:
    case 0x1D60F:
    case 0x1D643:
    case 0x1D677:
      putchar(0x48);		/* H */
      break;
    case 0x2110:
    case 0x2111:
    case 0x2160:
    case 0xFF29:
    case 0x1D408:
    case 0x1D43C:
    case 0x1D470:
    case 0x1D4D8:
    case 0x1D540:
    case 0x1D574:
    case 0x1D5A8:
    case 0x1D5DC:
    case 0x1D610:
    case 0x1D644:
    case 0x1D678:
      putchar(0x49);		/* I */
      break;
    case 0x026A:
    case 0xFF2A:
    case 0x1D409:
    case 0x1D43D:
    case 0x1D471:
    case 0x1D4D9:
    case 0x1D541:
    case 0x1D575:
    case 0x1D5A9:
    case 0x1D5DD:
    case 0x1D611:
    case 0x1D645:
    case 0x1D679:
      putchar(0x4A);		/* J */
      break;
    case 0xFF2B:
    case 0x1D40A:
    case 0x1D43E:
    case 0x1D472:
    case 0x1D4A5:
    case 0x1D4DA:
    case 0x1D50D:
    case 0x1D542:
    case 0x1D576:
    case 0x1D5AA:
    case 0x1D5DE:
    case 0x1D612:
    case 0x1D646:
    case 0x1D67A:
      putchar(0x4B);		/* K */
      break;
    case 0xFF2C:
    case 0x1D40B:
    case 0x1D43F:
    case 0x1D473:
    case 0x1D4A6:
    case 0x1D4DB:
    case 0x1D50E:
    case 0x1D543:
    case 0x1D577:
    case 0x1D5AB:
    case 0x1D5DF:
    case 0x1D613:
    case 0x1D647:
    case 0x1D67B:
      putchar(0x4C);		/* L */
      break;
    case 0x029F:
    case 0xFF2D:
    case 0x2112:
    case 0x1D40C:
    case 0x1D440:
    case 0x1D474:
    case 0x1D4DC:
    case 0x1D50F:
    case 0x1D544:
    case 0x1D578:
    case 0x1D5AC:
    case 0x1D5E0:
    case 0x1D614:
    case 0x1D648:
    case 0x1D67C:
      putchar(0x4D);		/* M */
      break;
    case 0x2133:
    case 0xFF2E:
    case 0x1D40D:
    case 0x1D441:
    case 0x1D475:
    case 0x1D4A9:
    case 0x1D4DD:
    case 0x1D511:
    case 0x1D579:
    case 0x1D5AD:
    case 0x1D5E1:
    case 0x1D615:
    case 0x1D649:
    case 0x1D67D:
      putchar(0x4E);		/* N */
      break;
    case 0x2205:
    case 0xFF2F:
    case 0x1D40E:
    case 0x1D442:
    case 0x1D476:
    case 0x1D4AA:
    case 0x1D4DE:
    case 0x1D512:
    case 0x1D546:
    case 0x1D57A:
    case 0x1D5AE:
    case 0x1D5E2:
    case 0x1D616:
    case 0x1D64A:
    case 0x1D67E:
      putchar(0x4F);		/* O */
      break;
    case 0x2118:
    case 0x2119:
    case 0xFF30:
    case 0x1D40F:
    case 0x1D443:
    case 0x1D477:
    case 0x1D4AB:
    case 0x1D4DF:
    case 0x1D513:
    case 0x1D57B:
    case 0x1D5AF:
    case 0x1D5E3:
    case 0x1D617:
    case 0x1D64B:
    case 0x1D67F:
      putchar(0x50);		/* P */
      break;
    case 0x211A:
    case 0xFF31:
    case 0x1D410:
    case 0x1D444:
    case 0x1D478:
    case 0x1D4AC:
    case 0x1D4E0:
    case 0x1D514:
    case 0x1D57C:
    case 0x1D5B0:
    case 0x1D5E4:
    case 0x1D618:
    case 0x1D64C:
    case 0x1D680:
      putchar(0x51);		/* Q */
      break;
    case 0x0280:
    case 0x211B:
    case 0x211C:
    case 0x211D:
    case 0xFF32:
    case 0x1D411:
    case 0x1D445:
    case 0x1D479:
    case 0x1D4E1:
    case 0x1D57D:
    case 0x1D5B1:
    case 0x1D5E5:
    case 0x1D619:
    case 0x1D64D:
    case 0x1D681:
      putchar(0x52);		/* R */
      break;
    case 0xFF33:
    case 0x1D412:
    case 0x1D446:
    case 0x1D47A:
    case 0x1D4AE:
    case 0x1D4E2:
    case 0x1D516:
    case 0x1D54A:
    case 0x1D57E:
    case 0x1D5B2:
    case 0x1D5E6:
    case 0x1D61A:
    case 0x1D64E:
    case 0x1D682:
      putchar(0x53);		/* S */
      break;
    case 0xFF34:
    case 0x1D413:
    case 0x1D447:
    case 0x1D47B:
    case 0x1D4AF:
    case 0x1D4E3:
    case 0x1D517:
    case 0x1D54B:
    case 0x1D57F:
    case 0x1D5B3:
    case 0x1D5E7:
    case 0x1D61B:
    case 0x1D64F:
    case 0x1D683:
      putchar(0x54);		/* T */
      break;
    case 0xFF35:
    case 0x1D414:
    case 0x1D448:
    case 0x1D47C:
    case 0x1D4B0:
    case 0x1D4E4:
    case 0x1D518:
    case 0x1D54C:
    case 0x1D580:
    case 0x1D5B4:
    case 0x1D5E8:
    case 0x1D61C:
    case 0x1D650:
    case 0x1D684:
      putchar(0x55);		/* U */
      break;
    case 0xFF36:
    case 0x1D415:
    case 0x1D449:
    case 0x1D47D:
    case 0x1D4B1:
    case 0x1D4E5:
    case 0x1D519:
    case 0x1D54D:
    case 0x1D581:
    case 0x1D5B5:
    case 0x1D5E9:
    case 0x1D61D:
    case 0x1D651:
    case 0x1D685:
      putchar(0x56);		/* V */
      break;
    case 0xFF37:
    case 0x1D416:
    case 0x1D44A:
    case 0x1D47E:
    case 0x1D4B2:
    case 0x1D4E6:
    case 0x1D51A:
    case 0x1D54E:
    case 0x1D582:
    case 0x1D5B6:
    case 0x1D5EA:
    case 0x1D61E:
    case 0x1D652:
    case 0x1D686:
      putchar(0x57);		/* W */
      break;
    case 0xFF38:
    case 0x1D417:
    case 0x1D44B:
    case 0x1D47F:
    case 0x1D4B3:
    case 0x1D4E7:
    case 0x1D51B:
    case 0x1D54F:
    case 0x1D583:
    case 0x1D5B7:
    case 0x1D5EB:
    case 0x1D61F:
    case 0x1D653:
    case 0x1D687:
      putchar(0x58);		/* X */
      break;
    case 0x2144:
    case 0xFF39:
    case 0x1D418:
    case 0x1D44C:
    case 0x1D480:
    case 0x1D4B4:
    case 0x1D4E8:
    case 0x1D51C:
    case 0x1D550:
    case 0x1D584:
    case 0x1D5B8:
    case 0x1D5EC:
    case 0x1D620:
    case 0x1D654:
    case 0x1D688:
      putchar(0x59);		/* Y */
      break;
    case 0x29F5:
    case 0x29F9:
    case 0xFE68:
      putchar(0x5C);		/* Backslash */
      break;
    case 0x2124:
    case 0x2128:
    case 0xFF3A:
    case 0x1D419:
    case 0x1D44D:
    case 0x1D656:
    case 0x1D68A:
      putchar(0x61);		/* a */
      break;
    case 0xFF42:		/* b */
    case 0x1D41B:
    case 0x1D44F:
    case 0x1D483:
    case 0x1D4B7:
    case 0x1D4EB:
    case 0x1D51F:
    case 0x1D553:
    case 0x1D587:
    case 0x1D5BB:
    case 0x1D5EF:
    case 0x1D623:
    case 0x1D657:
    case 0x1D68B:
      putchar(0x62);
      break;
    case 0x1D04:		/* c */
    case 0xFF43:
    case 0x1D41C:
    case 0x1D450:
    case 0x1D484:
    case 0x1D4B8:
    case 0x1D4EC:
    case 0x1D520:
    case 0x1D554:
    case 0x1D588:
    case 0x1D5BC:
    case 0x1D5F0:
    case 0x1D624:
    case 0x1D658:
    case 0x1D68C:
      putchar(0x63);
      break;
    case 0x1D05:		/* d */
    case 0x2146:
    case 0xFF44:
    case 0x1D41D:
    case 0x1D451:
    case 0x1D485:
    case 0x1D4B9:
    case 0x1D4ED:
    case 0x1D521:
    case 0x1D555:
    case 0x1D589:
    case 0x1D5BD:
    case 0x1D5F1:
    case 0x1D625:
    case 0x1D659:
    case 0x1D68D:
      putchar(0x64);
      break;
    case 0x1D07:		/* e */
    case 0x212F:
    case 0x2147:
    case 0xFF45:
    case 0x1D41E:
    case 0x1D452:
    case 0x1D486:
    case 0x1D4EE:
    case 0x1D522:
    case 0x1D556:
    case 0x1D5BE:
    case 0x1D5F2:
    case 0x1D626:
    case 0x1D65A:
    case 0x1D68E:
      putchar(0x65);
      break;
    case 0xFF46:		/* f */
    case 0x1D41F:
    case 0x1D453:
    case 0x1D487:
    case 0x1D4BB:
    case 0x1D4EF:
    case 0x1D523:
    case 0x1D557:
    case 0x1D58B:
    case 0x1D5BF:
    case 0x1D5F3:
    case 0x1D627:
    case 0x1D65B:
    case 0x1D68F:
      putchar(0x66);
      break;
    case 0xFF47:		/* g */
    case 0x210A:
    case 0x1D420:
    case 0x1D454:
    case 0x1D488:
    case 0x1D4F0:
    case 0x1D524:
    case 0x1D558:
    case 0x1D58C:
    case 0x1D5C0:
    case 0x1D5F4:
    case 0x1D628:
    case 0x1D65C:
    case 0x1D690:
      putchar(0x67);
      break;
    case 0xFF48:		/* h */
    case 0x1D421:
    case 0x1D489:
    case 0x1D4BD:
    case 0x1D4F1:
    case 0x1D525:
    case 0x1D559:
    case 0x1D58D:
    case 0x1D5C1:
    case 0x1D5F5:
    case 0x1D629:
    case 0x1D65D:
    case 0x1D691:
      putchar(0x68);
      break;
    case 0x1D09:
    case 0x2071:
    case 0xFF49:
    case 0x2148:
    case 0x1D422:
    case 0x1D456:
    case 0x1D48A:
    case 0x1D4BE:
    case 0x1D4F2:
    case 0x1D526:
    case 0x1D55A:
    case 0x1D58E:
    case 0x1D5C2:
    case 0x1D5F6:
    case 0x1D62A:
    case 0x1D65E:
    case 0x1D692:
      putchar(0x69);		/* i */
      break;
    case 0x1D0A:
    case 0x2149:
    case 0xFF4A:
    case 0x1D423:
    case 0x1D457:
    case 0x1D48B:
    case 0x1D4BF:
    case 0x1D4F3:
    case 0x1D527:
    case 0x1D55B:
    case 0x1D58F:
    case 0x1D5C3:
    case 0x1D5F7:
    case 0x1D62B:
    case 0x1D65F:
    case 0x1D693:
      putchar(0x6A);		/* j */
      break;
    case 0x1D0B:		/* k */
    case 0xFF4B:
    case 0x1D424:
    case 0x1D458:
    case 0x1D48C:
    case 0x1D4C0:
    case 0x1D4F4:
    case 0x1D528:
    case 0x1D55C:
    case 0x1D590:
    case 0x1D5C4:
    case 0x1D5F8:
    case 0x1D62C:
    case 0x1D660:
    case 0x1D694:
      putchar(0x6B);
      break;
    case 0x2113:		/* l */
    case 0xFF4C:
    case 0x1D425:
    case 0x1D459:
    case 0x1D48D:
    case 0x1D4C1:
    case 0x1D4F5:
    case 0x1D529:
    case 0x1D55D:
    case 0x1D591:
    case 0x1D5C5:
    case 0x1D5F9:
    case 0x1D62D:
    case 0x1D661:
    case 0x1D695:
      putchar(0x6C);
      break;
    case 0x1D0D:		/* m */
    case 0xFF4D:
    case 0x1D426:
    case 0x1D45A:
    case 0x1D48E:
    case 0x1D4C2:
    case 0x1D52A:
    case 0x1D55E:
    case 0x1D592:
    case 0x1D5C6:
    case 0x1D5FA:
    case 0x1D62E:
    case 0x1D662:
    case 0x1D696:
      putchar(0x6D);
      break;
    case 0x207F:
    case 0xFF4E:
    case 0x1D427:
    case 0x1D45B:
    case 0x1D48F:
    case 0x1D4C3:
    case 0x1D4F7:
    case 0x1D52B:
    case 0x1D55F:
    case 0x1D593:
    case 0x1D5C7:
    case 0x1D5FB:
    case 0x1D62F:
    case 0x1D663:
    case 0x1D697:
      putchar(0x6E);		/* n */
      break;
    case 0x1D0F:		/* o */
    case 0x2134:
    case 0xFF4F:
    case 0x1D428:
    case 0x1D45C:
    case 0x1D490:
    case 0x1D4F8:
    case 0x1D52C:
    case 0x1D560:
    case 0x1D594:
    case 0x1D5C8:
    case 0x1D5FC:
    case 0x1D630:
    case 0x1D664:
    case 0x1D698:
      putchar(0x6F);
      break;
    case 0x1D18:		/* p */
    case 0xFF50:
    case 0x213C:
    case 0x1D429:
    case 0x1D45D:
    case 0x1D491:
    case 0x1D4C5:
    case 0x1D4F9:
    case 0x1D52D:
    case 0x1D561:
    case 0x1D595:
    case 0x1D5C9:
    case 0x1D5FD:
    case 0x1D631:
    case 0x1D665:
    case 0x1D699:
      putchar(0x70);
      break;
    case 0xFF51:		/* q */
    case 0x1D42A:
    case 0x1D45E:
    case 0x1D492:
    case 0x1D4C6:
    case 0x1D4FA:
    case 0x1D52E:
    case 0x1D562:
    case 0x1D596:
    case 0x1D5CA:
    case 0x1D5FE:
    case 0x1D632:
    case 0x1D666:
    case 0x1D69A:
      putchar(0x71);
      break;
    case 0xFF52:		/* r */
    case 0x1D42B:
    case 0x1D45F:
    case 0x1D493:
    case 0x1D4C7:
    case 0x1D4FB:
    case 0x1D52F:
    case 0x1D563:
    case 0x1D597:
    case 0x1D5CB:
    case 0x1D5FF:
    case 0x1D633:
    case 0x1D667:
    case 0x1D69B:
      putchar(0x72);
      break;
    case 0xFF53:		/* s */
    case 0x1D42C:
    case 0x1D460:
    case 0x1D494:
    case 0x1D4C8:
    case 0x1D4FC:
    case 0x1D530:
    case 0x1D564:
    case 0x1D598:
    case 0x1D5CC:
    case 0x1D600:
    case 0x1D634:
    case 0x1D668:
    case 0x1D69C:
      putchar(0x73);
      break;
    case 0x1D1B:		/* t */
    case 0xFF54:
    case 0x1D42D:
    case 0x1D461:
    case 0x1D495:
    case 0x1D4C9:
    case 0x1D4FD:
    case 0x1D531:
    case 0x1D565:
    case 0x1D599:
    case 0x1D5CD:
    case 0x1D601:
    case 0x1D635:
    case 0x1D669:
    case 0x1D69D:
      putchar(0x74);
      break;
    case 0x1D1C:		/* u */
    case 0xFF55:
    case 0x1D42E:
    case 0x1D462:
    case 0x1D496:
    case 0x1D4CA:
    case 0x1D4FE:
    case 0x1D532:
    case 0x1D566:
    case 0x1D59A:
    case 0x1D5CE:
    case 0x1D602:
    case 0x1D636:
    case 0x1D66A:
    case 0x1D69E:
      putchar(0x75);
      break;
    case 0x1D20:		/* v */
    case 0xFF56:
    case 0x1D42F:
    case 0x1D463:
    case 0x1D497:
    case 0x1D4CB:
    case 0x1D4FF:
    case 0x1D533:
    case 0x1D567:
    case 0x1D59B:
    case 0x1D5CF:
    case 0x1D603:
    case 0x1D637:
    case 0x1D66B:
    case 0x1D69F:
      putchar(0x76);
      break;
    case 0x1D21:		/* w */
    case 0x24B2:
    case 0x24E6:
    case 0xFF57:
    case 0x1D430:
    case 0x1D464:
    case 0x1D498:
    case 0x1D4CC:
    case 0x1D500:
    case 0x1D534:
    case 0x1D568:
    case 0x1D59C:
    case 0x1D5D0:
    case 0x1D604:
    case 0x1D638:
    case 0x1D66C:
    case 0x1D6A0:
      putchar(0x77);
      break;
    case 0xFF58:		/* x */
    case 0x1D431:
    case 0x1D465:
    case 0x1D499:
    case 0x1D4CD:
    case 0x1D501:
    case 0x1D535:
    case 0x1D569:
    case 0x1D59D:
    case 0x1D5D1:
    case 0x1D605:
    case 0x1D639:
    case 0x1D66D:
    case 0x1D6A1:
      putchar(0x78);
      break;
    case 0xFF59:
    case 0x1D432:
    case 0x1D466:
    case 0x1D49A:
    case 0x1D4CE:
    case 0x1D502:
    case 0x1D536:
    case 0x1D56A:
    case 0x1D59E:
    case 0x1D5D2:
    case 0x1D606:
    case 0x1D63A:
    case 0x1D66E:
    case 0x1D6A2:
      putchar(0x79);		/* y */
      break;
    case 0x1D22:		/* z */
    case 0xFF5A:
    case 0x1D433:
    case 0x1D467:
    case 0x1D49B:
    case 0x1D4CF:
    case 0x1D503:
    case 0x1D537:
    case 0x1D56B:
    case 0x1D59F:
    case 0x1D5D3:
    case 0x1D607:
    case 0x1D63B:
    case 0x1D66F:
    case 0x1D6A3:
      putchar(0x7A);
      break;
    case 0xFE5B:
    case 0xFF5B:		/* left curly bracket */
      putchar(0x7B);
      break;
    case 0xFF5C:		/* pipe */
      putchar(0x7C);
      break;
    case 0xFE5C:
    case 0xFF5D:		/* right curly bracket */
      putchar(0x7D);
      break;
    case 0xFF5E:		/* tilde */
      putchar(0x7E);
      break;
    default:
      return 0;
    }
  return 1;
} /* End of AscifyStyle */


int MakeAscii(UTF32 c) {
  switch(c)
    {
    case 0x00A2:	/* Cent sign */
      putchar('c');
      break;
    case 0x00A3:	/* Pound sign */
      putchar('#');
      break;
    case 0x00A5:	/* Yen sign */
      putchar('Y');
      break;
    case 0x00A9:	/* Copyright symbol */
      putchar('C');
      break;
    case 0x00AE:	/* Registered symbol */
      putchar('R');
      break;
    case 0x00BC:	/* One quarter */
    case 0x00BD:	/* One half */
    case 0x00BE:	/* Three quarters */
      putchar('-');
      break;
    case 0x00C6:	/* Ash */
      putchar('A');
      break;
    case 0x00E6:	/* ash */
      putchar('a');
      break;
    case 0x00DF:	/* Eszet */
      putchar('s');
      break;
    case 0x0132:	/* Ligature IJ */
      putchar('I');
      break;
    case 0x0133:	/* Ligature ij */
      putchar('i');
      break;
    case 0x0152:	/* Ligature OE */
      putchar('O');
      break;
    case 0x0153:	/* Ligature oe */
      putchar('o');
      break;
    case 0x01F1:	/* Capital DZ */
      putchar('D');
      break;
    case 0x01F2:	/* Mixed Dz */
      putchar('D');
      break;
    case 0x01F3:	/* Small DZ */
      putchar('d');
      break;
    case 0x02A6:	/* Small ts digraph */
      putchar('t');
      break;
    case 0x20AC:	/* Euro sign */
      putchar('E');
      break;
    case 0x2026:	/* Horizontal ellipsis */
    case 0x22EF:	/* Midline horizontal ellipsis */
      putchar('.');
      break;
    case 0x2190:	/* Leftwards arrow */
      putchar('<');
      break;
    case 0x2192:	/* Rightwards arrow */
      putchar('>');
      break;
    case 0x21D0:	/* Leftwards double arrow */
      putchar('<');
      break;
    case 0x21D2:	/* Rightwards double arrow */
      putchar('>');
      break;
    default:
      return 0;
  }
  return 1;
}

int ExpandToAscii(UTF32 c) {
  switch(c)
    {
    case 0x00A2:	/* Cent sign */
      putchar('c');
      putchar('e');
      putchar('n');
      putchar('t');
      break;
    case 0x00A3:	/* Pound sign */
      putchar('p');
      putchar('o');
      putchar('u');
      putchar('n');
      putchar('d');
      break;
    case 0x00A5:	/* Yen sign */
      putchar('y');
      putchar('e');
      putchar('n');
      break;
    case 0x00A9:	/* Copyright symbol */
      putchar('(');
      putchar('c');
      putchar(')');
      break;
    case 0x00AE:	/* Registered symbol */
      putchar('(');
      putchar('R');
      putchar(')');
      break;
    case 0x00BC:	/* One quarter */
      putchar('1');
      putchar('/');
      putchar('4');
      break;
    case 0x00BD:	/* One half */
      putchar('1');
      putchar('/');
      putchar('2');
      break;
    case 0x00BE:	/* Three quarters */
      putchar('3');
      putchar('/');
      putchar('4');
      break;
    case 0x00C6:	/* Ash */
      putchar('A');
      putchar('E');
      break;
    case 0x00E6:	/* ash */
      putchar('a');
      putchar('e');
      break;
    case 0x00DF:	/* Eszet */
      putchar('s');
      putchar('s');
      break;
    case 0x0132:	/* Ligature IJ */
      putchar('I');
      putchar('J');
      break;
    case 0x0133:	/* Ligature ij */
      putchar('i');
      putchar('j');
      break;
    case 0x0152:	/* Ligature OE */
      putchar('O');
      putchar('E');
      break;
    case 0x0153:	/* Ligature oe */
      putchar('o');
      putchar('e');
      break;
    case 0x01F1:	/* Capital DZ */
      putchar('D');
      putchar('Z');
      break;
    case 0x01F2:	/* Mixed Dz */
      putchar('D');
      putchar('z');
      break;
    case 0x01F3:	/* Small DZ */
      putchar('d');
      putchar('z');
      break;
    case 0x02A6:	/* Small ts digraph */
      putchar('t');
      putchar('s');
      break;
    case 0x20AC:	/* Euro sign */
      putchar('e');
      putchar('u');
      putchar('r');
      putchar('o');
      break;
    case 0x2026:	/* Horizontal ellipsis */
    case 0x22EF:	/* Midline horizontal ellipsis */
      putchar('.');
      putchar('.');
      putchar('.');
      break;
    case 0x2190:	/* Leftwards arrow */
      putchar('<');
      putchar('-');
      break;
    case 0x2192:	/* Rightwards arrow */
      putchar('-');
      putchar('>');
      break;
    case 0x21D0:	/* Leftwards double arrow */
      putchar('<');
      putchar('=');
      break;
    case 0x21D2:	/* Rightwards double arrow */
      putchar('=');
      putchar('>');
      break;
    case 0xFB00:
      putchar('f');
      putchar('f');
      break;
    case 0xFB01:
      putchar('f');
      putchar('i');
      break;
    case 0xFB02:
      putchar('f');
      putchar('l');
      break;
    case 0xFB03:
      putchar('f');
      putchar('f');
      putchar('i');
      break;
    case 0xFB04:
      putchar('f');
      putchar('f');
      putchar('l');
      break;
    case 0xFB06:
      putchar('s');
      putchar('t');
      break;
    default:
      return 0;
  }
  return 1;
}

void
CheckNumberError(char *pgname,char *s, char c){
  if(errno == ERANGE){
    fprintf(stderr,"%s: %s falls outside of the representable range\n",pgname,s);
    exit(OTHERERROR);
  }
  if(errno == EINVAL){
    fprintf(stderr,"%s: %s is ill-formed\n",pgname,s);
    exit(OTHERERROR);
  }
  if(c != '\0'){
    fprintf(stderr,"%s: %s is ill-formed\n",pgname,s);
    exit(OTHERERROR);
  }
}

struct subpair {
  UTF32 u;
  char  a;
};

struct subpair *SubList = NULL;
int SubCnt = 0;
int SubsAvailable = 0;

/*
 * If people start using lots of custom substitutions, a more efficient
 * search may be advisable. For the time being we just do a linear search.
 */

SubstituteChar(UTF32 c) {
  int i;
  for(i = 0; i < SubCnt; i++) {
    if(c == SubList[i].u) {
      if(SubList[i].a) {
	putchar(SubList[i].a);
	return 1;
      }
      else return (-1);		/* Signal deletion */
    }
  }
  return 0;
}

AddCustomSubstitution(char *str){
  char *Left;
  char *Right;
  char *Delim;
  char *endptr;
  unsigned long UCode;
  unsigned long ACode;

  Delim = strchr(str,':');
  if(Delim == NULL) {
    fprintf(stderr,"Substition specification %s is ill-formed.\n",str);
    exit(BADOPTIONARG);
  }
  *Delim = '\0';
  Left = str;
  Right = Delim+1;
  UCode = strtoul(Left,&endptr,0);
  CheckNumberError(pgname,Left,*endptr);
  if(*Right == '\0') ACode = 0;
  else {
    ACode = strtoul(Right,&endptr,0);
    CheckNumberError(pgname,Right,*endptr);
  }
  if(ACode > 127L) {
    fprintf(stderr,"Ascii code for substitution must be between 1 and 127 inclusive.\n");
    exit(BADOPTIONARG);
  }
  SubCnt++;
  if(SubCnt > SubsAvailable) {
    SubList = realloc(SubList,sizeof(struct subpair) * (SubsAvailable+=4));
    if(SubList == NULL) {
      fprintf(stderr,"Unable to allocate storage for substitution list.\n");
      exit(OUTOFMEMORY);
    }
  }
  SubList[SubCnt-1].u = (UTF32) UCode;
  SubList[SubCnt-1].a = (char)  ACode;
}

int main (int ac, char *av[])
{

  UTF32 c;
  char sc;			/* Unicode char stripped to ASCII equivalent */
  int ch;
  int oc;			/* Command line option flag */
  int ucnt;			/* Index into current UTF* string  */
  int UCBytes;
  int infd;
  int FType = STDX;
  short UseEntitiesP = 0;
  int UTF8Type = 0;
  short BMPSplitP = 0;
  char *fmt = NULL;
  short dummy;			/* For compatibility with ascii2uni */

  unsigned char b1,b2,b3; /* The low three bytes of a 4 byte UTF-32 character */

  unsigned long ByteCnt;
  unsigned long CharCnt;
  unsigned long ConvertedCnt;
  unsigned long SubstitutedCnt;
  unsigned long DeletedCnt;

  short PureP = 0;
  short PreserveNewlinesP = 1;
  short PreserveSpacesP = 1;
  short VerboseP = 1;
  short HexUpperP = 1;	/* Use X or x in  hex ?  */
  short AddWhitespaceP = 0;
  short PassThroughP = 0;	/* If untransformed, pass through unicode rather than asciifying */
  short AllHTMLP = 0;		/* Not a valid option here - just used to detect bad cl */

  short AscifyStyleP = 0;
  short AscifyDiacriticsP = 0;
  short AscifyEnclosedP = 0;
  short AscifyEquivP = 0;
  short ExpandP = 0;
  short MakeP = 0;
  short LittleEndianP = 0;

  short convp = 1;		/* Just for local use due to complex logic */

  char *AboveBMPfmt;
  char *WithinBMPfmt;
  char *e;
  unsigned char *ustr;	/* UTF8 string */

  extern int optind;
  extern int opterr;
  extern int optopt;
  extern UTF32 UTF8in(int,int *,unsigned char **);

  extern int CountSlots(char *);
  extern void ListFormatArguments(short);
  extern void SetFormat(char *, int *, short *,int *, short *, short *);

  opterr = 0;
  ByteCnt = 0L;
  CharCnt = 0L;
  ConvertedCnt = 0L;
  SubstitutedCnt = 0L;
  DeletedCnt = 0L;

#ifdef HAVE_SETLOCALE
   setlocale(LC_ALL,"");
#endif
#ifdef HAVE_LIBINTL_H
   bindtextdomain (PACKAGE,LOCALEDIR);
   textdomain (PACKAGE);
#endif

  /* Handle command line arguments */

  while( (oc = getopt(ac,av,":Aa:BcdeEfhlLnPpqsS:vwyxZ:")) != EOF){
    switch(oc){
    case 'a':
      SetFormat(optarg,&FType,&UseEntitiesP, &UTF8Type, &BMPSplitP,&AllHTMLP);
       if( (FType == FMT_UNKNOWN) || AllHTMLP) {
	 fprintf(stderr,"Format specification %s not recognized.\n",optarg);
	 exit(BADOPTIONARG);
       }
       if(FType == CHENT) FType = HTMLX;
      break;
    case 'A':
      ListSingleApproximations();
      exit(INFO);
    case 'B':
      AscifyEnclosedP = 1;
      AscifyDiacriticsP = 1;
      AscifyEquivP = 1;
      AscifyStyleP = 1;
      ExpandP = 1;
      break;
    case 'c':
      AscifyEnclosedP = 1;
      break;
    case 'd':
      AscifyDiacriticsP = 1;
      break;
    case 'E':
      ListExpansions();
      exit(INFO);
    case 'e':
      AscifyEquivP = 1;
      break;
    case 'f':
      AscifyStyleP = 1;
      break;
    case 'l':
      HexUpperP = 0;
      break;
    case 'L':
      ListFormatArguments(1);
      exit(INFO);
    case 'n':
      PreserveNewlinesP = 0;
      break;
    case 'P':
      PassThroughP = 1;
      UTF8Type = 0;
      break;
    case 'p':		/* Convert even characters within the ASCII range */
      PureP =1;
      break;
    case 's':
      PreserveSpacesP = 0;
      break;
    case 'S':
      AddCustomSubstitution(optarg);
      break;
    case 'x':
      ExpandP = 1;
      break;
    case 'y':
      MakeP = 1;
      break;
    case 'w':
      AddWhitespaceP = 1;
      break;
    case 'Z':
      fmt = optarg;
      if(CountSlots(fmt) > 1) {
	fprintf(stderr,"You may not supply a format with more than one empty slot.\n");
	exit(BADOPTIONARG);
      }
      break;
    case 'h':
      ShowUsage();
      exit(INFO);
      break; 			/* NOTREACHED */
    case 'q':
      VerboseP = 0;
      break;
    case 'v':
      ShowVersion(stderr);
      Copyright();
      exit(INFO);
      break; 			/* NOTREACHED */
     case ':':
       fprintf(stderr,_("%s: missing argument to option flag %c.\n"),pgname,optopt);
       exit(BADOPTIONARG);
    default:
      fprintf(stderr,_("%1$s: invalid option flag %2$c\n"),pgname,optopt);
      ShowVersion(stderr);
      Copyright();
      ShowUsage();
      exit(BADOPTION);
    }
  } 

  if(optind < ac) {
    infd = open(av[optind],O_RDONLY);
    if (infd < 0) {
      fprintf(stderr,"unable to open input file %s\n",av[optind]);
      exit(OPENERROR);
    }
  }
  else infd = fileno(stdin);


  if(VerboseP) {
    if(SubCnt) fprintf(stderr,"Custom substitutions defined: %d\n",SubCnt);
  }

  /* Combinatorial checks */

  if(UseEntitiesP && !(FType == HTMLX || FType == HTMLD || FType == HDML)) {
    fprintf(stderr,"Use of character entities must be combined with either\ndecimal or hexadecimal HTML character references\nor HDML hexadecimal character references.\n");
    exit(BADOPTION);
  }

  if ( (FType == BYTEO) || (FType == BYTED) || (FType == BYTEH)) {
    LittleEndianP = Get_Endianness();
    switch (LittleEndianP)
      {
      case E_BIG_ENDIAN:
	LittleEndianP = 0;
	break;
      case E_LITTLE_ENDIAN:
	break;
      default:
	fprintf(stderr,"This machine is neither big-endian nor little-endian.\n");
	exit(OTHERERROR);
      }
  }

  if(fmt == NULL) {
    fmt = Formats[(2 * FType) + HexUpperP];
  }

  if(HexUpperP) {
    AboveBMPfmt  = "\\U%08X"; 
    WithinBMPfmt = "\\u%04X";  
  }
  else {
    AboveBMPfmt  = "\\U%08x";
    WithinBMPfmt = "\\u%04x";
  }

  if(UTF8Type) {
    if(AscifyStyleP|AscifyDiacriticsP|AscifyEquivP|AscifyEnclosedP|ExpandP|MakeP) {
      while ( (c = UTF8in(infd,&UCBytes,&ustr)) <= UNI_MAX_UTF32) {
	ByteCnt+=UCBytes;
	CharCnt++;
	if(SubCnt) {
	  switch(SubstituteChar(c))
	    {
	    case 1:
	      SubstitutedCnt++;
	      continue;
	    case -1:
	      DeletedCnt++;
	      continue;
	    }
	}
	if(AscifyStyleP) {
	  if(AscifyStyle(c))  {
	    SubstitutedCnt++;
	    continue;
	  }
	} 
	if(AscifyDiacriticsP) {
	  if(AscifyDiacritics(c))  {
	    SubstitutedCnt++;
	    continue;
	  }
	} 
	if(ExpandP) {
	  if(ExpandToAscii(c)) {
	    SubstitutedCnt++;
	    continue;
	  }
	}
	if(MakeP) {
	  if(MakeAscii(c)) {
	    SubstitutedCnt++;
	    continue;
	  }
	}
	if(AscifyEquivP) {
	  if(AscifyEquiv(c)) {
	    SubstitutedCnt++;
	    continue;
	  }
	} 
	if(AscifyEnclosedP) {
	  if(AscifyEnclosed(c)) {
	    SubstitutedCnt++;
	    continue;
	  }
	} 
	ucnt= 0;
	while(ucnt < UCBytes) {
	  ch = ustr[ucnt++];
	  convp = 1;
	  if(ch <= 0x7F) convp = 0;
	  switch(ch) {
	    case 0x0A:
	      if(!PreserveNewlinesP) convp = 1;
	      break;
	    case 0x20:
	      if(!PreserveSpacesP) convp = 1;
	      break;
	    default:
	      if(PureP) convp = 1;
	    }
	  if (!convp) putchar(ch);
	  else {
	    ConvertedCnt++;
	    switch (UTF8Type) {
	    case 1:
	      printf("%%%02X",ch);
	      break;
	    case 2:
	      printf("=%02X",ch);
	      break;
	    case 3:
	      printf("\\%03o",ch);
	      break;
	    case 4:
	      printf("\\x%02X",ch);
	      break;
	    case 5:
	      printf("<%02X>",ch);
	      break;
	    }
	  }
	  if(AddWhitespaceP) putchar(' ');
	}
      }
      exit(SUCCESS);
    }
    else {
      while( (ch = getchar()) != EOF) { 
	convp = 1;
	if(ch <= 0x7F) convp = 0;
	switch(ch) {
	  case 0x0A:
	    if(!PreserveNewlinesP) convp = 1;
	    break;
	  case 0x20:
	    if(!PreserveSpacesP) convp = 1;
	    break;
	  default:
	    if(PureP) convp = 1;
	  }
	if (!convp) putchar(ch);
	else {
	  ConvertedCnt++;
	  switch (UTF8Type) {
	  case 1:
	    printf("%%%02X",ch);
	    break;
	  case 2:
	    printf("=%02X",ch);
	    break;
	  case 3:
	    printf("\\%03o",ch);
	    break;
	  case 4:
	    printf("\\x%02X",ch);
	    break;
	  case 5:
	    printf("<%02X>",ch);
	    break;
	  }
	}
	if(AddWhitespaceP) putchar(' ');
      }
      exit(SUCCESS);
    }
  } /* End of UTF8 types */

  while ( (c = UTF8in(infd,&UCBytes,&ustr)) <= UNI_MAX_UTF32){
    ByteCnt+=UCBytes;
    CharCnt++;
    if(SubCnt) {
      switch(SubstituteChar(c))
	{
	case 1:
	  SubstitutedCnt++;
	  continue;
	case -1:
	  DeletedCnt++;
	  continue;
	}
    }
    if(AscifyStyleP) {
      if(AscifyStyle(c))  {
	SubstitutedCnt++;
	continue;
      }
    } 
    if(AscifyDiacriticsP) {
      if(AscifyDiacritics(c))  {
	SubstitutedCnt++;
	continue;
      }
    } 
    if(ExpandP) {
      if(ExpandToAscii(c)) {
	SubstitutedCnt++;
	continue;
      }
    }
    if(MakeP) {
      if(MakeAscii(c)) {
	SubstitutedCnt++;
	continue;
      }
    }
    if(AscifyEquivP) {
      if(AscifyEquiv(c)) {
	SubstitutedCnt++;
	continue;
      }
    } 
    if(AscifyEnclosedP) {
      if(AscifyEnclosed(c)) {
	SubstitutedCnt++;
	continue;
      }
    } 
    if ( (FType == HDML) && (c > 0xFF)) {
      fprintf(stderr,"Character 0x%04x encountered at character %lu skipped.\n"
	      ,(unsigned int) c,CharCnt);
      fprintf(stderr,"   Characters above 0xFF cannot be represented in HDML.\n");
      DeletedCnt++;
      continue;
    }
    switch (c) {
    case 0x000A:
      if(PreserveNewlinesP) putchar((int)c);
      else {
	printf(fmt,c);
	if(AddWhitespaceP) putchar(' ');
	ConvertedCnt++;
      }
      break;
    case 0x0020:		/* space */
    case 0x0009:		/* tab */
      if(PreserveSpacesP)  putchar((int)c);
      else {
	printf(fmt,c);
	if(AddWhitespaceP) putchar(' ');
	ConvertedCnt++;
      }
      break;
    case 0x1361:		/* ethiopic word space */
    case 0x1680:		/* ogham space */
    case 0x3000:		/* ideographic space */
      if(PreserveSpacesP) putchar(0x0020);
      else {
	printf(fmt,c);
	if(AddWhitespaceP) putchar(' ');
	ConvertedCnt++;
      }
      break;
    default:
      if(PassThroughP) putu8(c);
      else if(!PureP && (c <= 0x7F)) putchar((int)c);
      else {
	if(BMPSplitP) {
	  if(c > 0xFFFF) printf(AboveBMPfmt,c);
	  else printf(WithinBMPfmt,c);
	  ConvertedCnt++;
	}
	else {
	  if (UseEntitiesP) {
	    if (FType == HDML) {
	      if ((e=LookupHDMLEntityForCode(c)) != NULL) printf("&%s;",e);
	      else printf(fmt,c);
	    }
	    else {
	      if ((e=LookupEntityForCode(c)) != NULL) printf("&%s;",e);
	      else printf(fmt,c);
	    }
	    ConvertedCnt++;
	  }
	  else {
	    ConvertedCnt++;
	    if ( (FType == BYTEO) || (FType == BYTED) || (FType == BYTEH)) {
	      /* Single byte UTF-32*/
	      if(LittleEndianP) lswab(&c); /* POSIX assumes big-endian */
	      /* Extract the low three bytes */
	      b1 = (char) ((c >> 16) & 0xFF);
	      b2 = (char) ((c >>  8) & 0xFF);
	      b3 = (char) ( c        & 0xFF);
	      printf(fmt,b1,b2,b3);
	    }
	    else printf(fmt,c);
	  }
	}
	if(AddWhitespaceP) putchar(' ');
      }
    }
  }

  switch (c){ 
  case UTF8_NOTENOUGHBYTES:
    fprintf(stderr,_("Truncated UTF-8 sequence encountered at byte %1$lu, character %2$lu.\n"),
	    ByteCnt,CharCnt);
    exit(BADRECORD);
    break;
  case UTF8_BADINCODE:
    fprintf(stderr,_("Invalid UTF-8 code encountered at byte %1$lu, character %2$lu.\n"),
	    ByteCnt,CharCnt);
    exit(BADRECORD);
    break;
  case UTF8_BADOUTCODE:
    fprintf(stderr,_("Encountered invalid Unicode at byte %1$lu, character %2$lu.\n"),
	    ByteCnt,CharCnt);
    exit(BADRECORD);
    break;
  case UTF8_IOERROR:
    snprintf(msg,MSGSIZE-1,_ ("Error reading input at byte %1$lu, character %2$lu.\n"),
	     ByteCnt,CharCnt);
    perror(msg);
    exit(IOERROR);
    break;
  default:			/* Normal EOF */
    break;
  }

#ifdef NEWSUMMARY
  if(VerboseP) {
    fprintf(stderr,_("Total input characters          %12ld\n"),CharCnt);
    fprintf(stderr,_("Characters converted to escapes %12ld\n"),ConvertedCnt);
    fprintf(stderr,_("Characters replaced with ASCII  %12ld\n"),SubstitutedCnt);
    fprintf(stderr,_("Characters deleted              %12ld\n"),DeletedCnt);
  }
#else  
  if(VerboseP) fprintf(stderr,_("%1$ld tokens converted out of %2$ld characters\n"),ConvertedCnt,CharCnt);
#endif
  exit(SUCCESS);
}


