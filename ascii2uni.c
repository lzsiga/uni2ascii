/* Time-stamp: <2011-02-16 10:44:02 poser>
 *
 * Convert text containing various 7-bit ASCII escapes to UTF-8 Unicode.
 *
 * Copyright (C) 2005-2011 William J. Poser (billposer@alum.mit.edu)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 3 of the GNU General Public License
 * as published by the Free Software Foundation.
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
#include <string.h>
#include <ctype.h>
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
#include "exitcode.h"
#include "formats.h"

#if defined(__DATE__) && defined(__TIME__)
#define HAVE_DATE_TIME
char compdate[]= __DATE__ " " __TIME__ ;
#else
char compdate[]= "";
#endif

char version[]=PACKAGE_VERSION;
char pgname[]="ascii2uni";

#define LBUFINIT 128

#ifndef LOCALEDIR 
#define LOCALEDIR "/usr/local/share/locale"
#endif

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

void Copyright() {
  fprintf(stderr,"Copyright (C) 2004-2011 William J. Poser\n");
  fprintf(stderr,"This program is free software; you can redistribute\n\
it and/or modify it under the terms of version 3 of\n\
the GNU General Public License as published by the\n\
Free Software Foundation.\n");
  fprintf(stderr,"Report bugs to: billposer@alum.mit.edu.\n");
}


void
ShowUsage(void){
  fprintf(stderr,_("This program is a filter which converts 7-bit ASCII text\n\
containing various representations for non-ASCII characters\nto UTF-8 Unicode.\n"));
  fprintf(stderr,_("Usage: %s [flags] (<input filename>)\n"),pgname);
  fprintf(stderr,_("       -a <format specification>.\n"));
  fprintf(stderr,_("       -h Print this usage message.\n"));
  fprintf(stderr,_("       -L List format codes.\n"));
  fprintf(stderr,_("       -m Accept Microsoft-style HTML entities w/o semi-colon.\n"));
  fprintf(stderr,_("       -p Input consists of pure escapes except for non-null whitespace.\n"));
  fprintf(stderr,_("       -q Quiet - don't chat.\n"));
  fprintf(stderr,_("       -v Print version information.\n"));
  fprintf(stderr, 
	  _("       -Z <format> Convert input using the supplied format.\n"));
  fprintf(stderr,_("Report bugs to: billposer@alum.mit.edu\n"));
}


char *
ExtractSubstring(char *strptr, char* Start, char* End) {
  char *i;
  char *SavedBeginning;

  SavedBeginning = strptr;
  for (i = Start; i <= End; i++) *strptr++ = *i;
  *strptr = '\0';
  return SavedBeginning;
}


/* The length of the longest character entity */
#define MAXENTLEN 8

/* The library function seems not to be working. Anyhow, we want to keep this
 * independent of locale.
 */
int myisxdigit (int c) {
  switch (c) {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
    return 1;
  default:
    return 0;
  }
}

static char *Formats [] = {
"&#x%lX;", 	/* HTMLX */
"&#%ld;", 	/* HTMLD */
"\\#x%lX;", 	/* SGMLX */
"\\#%ld;",	/* SGMLD */
"\\u%8lX",	/* BSLU */
"\\x%lX",	/* BSLX */
"0x%4lX",	/* STDX */
"#x%4lX",	/* CLSX */
"%lX", 		/* RAWX */
"\\x{%lX}",	/* BSLXB */
"<U%lX>",	/* ABUX */
"U%lX",		/* JUUX */
"u%lX",		/* JuUX */
"U+%lX",	/* UPLX */
"X\'%lX\'",	/* XQ */
"\\u%8ld",	/* BSLUD */
"v%ld",		/* PERLV */
"$%04X",	/* DOLLAR */
"16#%04X",	/* PSPT */
"#16r%04X",	/* CLR */
"16#%04X#",	/* ADA */
"&#02X;",	/* HDML */
"\\%03o\\%03o\\%03o",	/* BYTEO */
"\\d%03d\\d%03d\\d%03d",	/* BYTED */
"\\x%02x\\x%02x\\x%02x",	/* BYTEX */
"_x%4lX_",	/* OOXML */
"%%u%lX",		/* PCTUX */
"&%[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789];", /* CHENT */
"=%2lX", 		/* UTF-8  - Ifmt */
"%%%2lX", 		/* UTF-8  - Jfmt */
"\\%3lo", 		/* UTF-8  - Kfmt */
"\\x%2lX", 		/* UTF-8  - APACHE */
"<%2lX>" 		/* UTF-8  - UTF8ANGLE */
};

#define AFMTSIZE (67+2+1+2)

int main (int ac, char *av[])
{
  char *SplitFormat = "\\%1[uU]%X%n"; /* This is for BMPSplit */

  char *fmt = Formats[STDX];		/* Default is plain hex format */
  char afmt [AFMTSIZE];
  char aHfmt [8+2+1];
  char aDfmt [8+2+1];
  char cbuf[5];
  FILE *infp;

  UTF32 num;
  int oc;			/* Command line option flag */
  int Converted;
  long TokenNumber = 0L;
  long ReplacedNumber = 0L;
  short BMPSplitP = 0;
  int VerboseP = 1;
  int UTF8ValueP = 0;		/* Are incoming values UTF-8? */
  short AllHTMLP = 0; 		/* Translate all three kinds of HTML escape */
  int PureP = 0;
  int StrictP = 1;		/* Do not convert Microsoft-style numeric character references */
  long MicrosoftStyle = 0L;	/* Number of Microsoft-style ncrs detected */
  int Word_Length;
  int NConsumed;
  unsigned long LineNo;
  char *str;
  char *iptr;
  int eof;
  char SplitStr[3];
  char enam[16];
  char tmpstr [16];
  unsigned char b1;		/* Used for byte-wise encoding */
  unsigned char b2;
  unsigned char b3;
  int FType = STDX;
  int UTF8Type;			/* Not used - for compatibility with uni2ascii */
  short UseEntitiesP;	/* Not used - for compatibility with uni2ascii */
  int last;
  size_t len = LBUFINIT;
  ssize_t read;
  short AddNewlineP=1;		/* Used with quoted-printable end-of-line =-sign */

  char *lbuf = NULL;

  extern int optind;
  extern int opterr;
  extern int optopt;
  extern unsigned long GetWordLineNo;

  extern void putu8 (unsigned long);
  extern char * Get_Word(FILE *, int *, int *);
  extern int CountSlots(char *);
  extern void ListFormatArguments(short);
  extern void SetFormat(char *, int *, short *,int *, short *, short *);

#ifdef DEBUGBUILD
  fprintf(stderr,"Execution has begun.\n");fflush(stderr);
#endif

  opterr = 0;

#ifdef HAVE_SETLOCALE
   setlocale(LC_ALL,"");
#endif
#ifdef HAVE_LIBINTL_H
   bindtextdomain (PACKAGE,LOCALEDIR);
   textdomain (PACKAGE);
#endif

  /* Handle command line arguments */

   while( (oc = getopt(ac,av,":Z:a:hLmpqv")) != EOF){
     switch(oc){
     case 'a':
       SetFormat(optarg,&FType,&UseEntitiesP, &UTF8Type, &BMPSplitP,&AllHTMLP);
       if(FType == FMT_UNKNOWN) {
	 fprintf(stderr,"Format specification %s not recognized.\n",optarg);
	 exit(BADOPTIONARG);
       }
       fmt = Formats[FType];
       if( (FType == IFMT) || (FType == JFMT) ||
	   (FType == KFMT) || (FType == APACHE) ||
	   (FType == UTF8ANGLE)) UTF8ValueP =1;
       if( (FType == JFMT) || (FType == UTF8ANGLE)) {cbuf[0] = '0'; cbuf[1] = 'x';}
       if(FType == KFMT) {cbuf[0] = '\\';}
       if(FType == APACHE) {cbuf[0] = '0';}
       break;
    case 'L':
      ListFormatArguments(0);
      exit(INFO);
     case 'Z':
       fmt = optarg;
       if(CountSlots(fmt) > 1) {
	 fprintf(stderr,_("You may not supply a format with more than one empty slot.\n"));
	 exit(BADOPTIONARG);
       }
       break;
     case 'm':
       StrictP = 0;
       break;
     case 'p':
       PureP = 1;
       break;
     case 'q':
       VerboseP = 0;
       break;
     case 'h':
       ShowUsage();
       exit(INFO);
       break; 			/* NOTREACHED */
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
       exit(INFO);
     }
   }

   if(optind < ac) {
     infp = fopen(av[optind],"r");
     if (infp == NULL) {
       fprintf(stderr,"unable to open input file %s\n",av[optind]);
       exit(OPENERROR);
     }
   }
   else infp = stdin;

#ifdef DEBUGBUILD
   fprintf(stderr,"Command-line arguments processed.\n");fflush(stderr);
#endif

   if( (FType == RAWX) && (!PureP) ) {
     fprintf(stderr,_("It isn't possible reliably to parse raw hex unicode out of ASCII text.\n"));
     exit(BADOPTION);
   }

   if(AllHTMLP && PureP) {
     fprintf(stderr,_("Conversion of all three HTML formats is not supported in pure mode.\n"));
     exit(BADOPTION);
   }

#ifdef DEBUGBUILD
   fprintf(stderr,"Sanity checks completed.\n");fflush(stderr);
#endif

   if(AllHTMLP) {
     sprintf(aDfmt,"%s%%n",Formats[HTMLD]);
     sprintf(aHfmt,"%s%%n",Formats[HTMLX]);
   }

#ifdef DEBUGBUILD
   fprintf(stderr,"fmt = %s\n",fmt);fflush(stderr); /* debug */
#endif

   snprintf(afmt,AFMTSIZE,"%s%%n",fmt);	/* Add %n for NConsumed */

   /*
    * This is the case in which the input consists entirely of escapes
    * except for arbitrary (but non-null) amounts of intervening whitespace.
    */

#ifdef DEBUGBUILD
   fprintf(stderr,"Setup completed.\n");fflush(stderr); /* debug */
#endif
   if(PureP) {
     GetWordLineNo = 1;
     while(1){
       str = Get_Word(infp,&Word_Length,&eof);
       if(eof) break; 
       if(Word_Length == 0) continue;
       TokenNumber++;
       if(str == NULL){
	 fprintf(stderr,_("%1$s: failed to allocate storage for input token %2$ld at line %3$lu.\n"),
		 pgname,TokenNumber,GetWordLineNo);
	 exit(OUTOFMEMORY);
       }
       if(FType == CHENT) {
	 Converted = sscanf(str,afmt,&enam,&NConsumed);
	 num = LookupCodeForEntity(enam);
	 if(!num) {
	   num = UNI_REPLACEMENT_CHAR;
	   fprintf(stderr,"ascii2uni: unknown HTML character entity \"&%s;\" at line %lu\n",
		   enam,GetWordLineNo);
	   ReplacedNumber++;
	   Converted = (-1);
	 }
	 else Converted = 1;
       }
       else if( (BYTEO == FType) || (BYTED == FType) || (BYTEH == FType) || (UTF8ANGLE == FType)) {
	 Converted = sscanf(str,afmt,&b1,&b2,&b3,&NConsumed);
	 switch(Converted)
	   {
	   case 3:
	     num = (((b1 * 256) + b2) * 256) + b3;
	     break;
	   case 2:
	     num = (b1 * 256) + b2;
	     break;
	   case 1:
	     num = b1;
	     break;
	   default:
	     break;
	     /* This case is handled below */
	 }
       }
       else {
	 Converted = sscanf(str,afmt,&num,&NConsumed);
       }

       if(Converted < 1) {
	 fprintf(stderr,_("Ill-formed input %1$s at token %2$lu at line %3$lu\n"),
		 str,TokenNumber,GetWordLineNo);
	 exit(BADRECORD); 
       }
       else if(Converted > 3) {
	 fprintf(stderr,_("The character encoded as %1$s at token %2$lu at line %3$lu is outside the Unicode range.\n\tEmitting Unicode replacement character.\n"),
		 str,TokenNumber,GetWordLineNo);
	 putu8(UNI_REPLACEMENT_CHAR);
       } 
       else {
	 if( (FType == HTMLD) || (FType == HTMLX) || (FType == CHENT) || (FType == HDML)) {
	   if(*(str+NConsumed-1) != ';') {
	     MicrosoftStyle++;
	     fprintf(stderr,_("The HTML/HDML entity %1$s at token %2$lu at line %3$lu lacks the requisite final semicolon.\n"),
		     str,TokenNumber,GetWordLineNo);
	     if(StrictP) {
	       fputs(str,stdout);
	       TokenNumber--;
	     } else {
	       if (UTF8ValueP) putchar(num);
	       else putu8(num);
	     }
	     free((void *)str);
	     continue;
	   }
	 }
	 if (UTF8ValueP) putchar(num);
	 else putu8(num);
       }
       free((void *)str);
     }
     goto done;
   } /* End of PureP */

   /* This is the case in which the Unicode escapes are embedded in ASCII text */

   LineNo = 0;

   lbuf= (char *) malloc(len);
   if(lbuf == NULL) {
     fprintf(stderr,"Failed to allocate buffer for input line.\n");
     exit(2);
   }
   while ((read = getline(&lbuf, &len, infp)) != -1) {
     AddNewlineP = 1;
     LineNo++;
     last = read - 1;
     if(lbuf[last] == '\n') {
       lbuf[last] = '\0';
       last--;
     }
     if(last < 0) {
       putchar('\n');
       continue;
     }
     if (FType == IFMT) {	/* Quoted-printable */
       if (lbuf[last] == '=') {
	 lbuf[last] = '\0';
	 AddNewlineP = 0;
       }
     }
     iptr = lbuf;
     if(FType == JFMT) {
       while(*iptr) {
	 if(*iptr == '%') {
	   if(*++iptr) {
	     if(myisxdigit(*iptr)) {
	       if(*++iptr) {
		 if(myisxdigit(*iptr)) { /* match */
		   cbuf[2] = *(iptr-1);
		   cbuf[3] = *iptr;
		   cbuf[4] = '\0';
		   num = (unsigned char)strtoul(cbuf,NULL,16);
		   putchar(num);
		   TokenNumber++;
		   iptr++;
		 }
		 else {		/* We have % X foo */
		   putchar('%');
		   putchar(*(iptr-1));
		   if(*iptr != '%') putchar(*iptr++);
		   continue;
		 }
	       }
	       else {		/* We have % X EOL */
		 putchar('%');
		 putchar(*(iptr-1));
		 putchar('\n');
		 break;
	       }
	     }
	     else { 		/* We have % foo */
		 putchar('%');
		 if(*iptr != '%') putchar(*iptr++);
		 continue;
	     }
	   }	     
	   else {		/* We have % EOL */
	     putchar('%');
	     putchar('\n');
	     break;
	   }
	 }
	 else {
	   putchar(*iptr++);
	   continue;
	 }
       }
     } /* End of special case for J format */

     while (*iptr) {
       if(BMPSplitP) {
	 if(sscanf(iptr,SplitFormat,&SplitStr,&num,&NConsumed)) {
	   if( (num <= 0xFFFF) && (SplitStr[0] == 'U')) {
	     fprintf(stderr,_("Warning: the code \\U%1$08lX at line %2$lu falls within the BMP.\n"),
		     num,LineNo);
	   }
	   if( (num > 0xFFFF) && (SplitStr[0] == 'u')) {
	     fprintf(stderr,_("Warning: the code \\u%1$08lX at line %2$lu falls outside the BMP.\n"),
		     num,LineNo);
	   }
	   putu8(num);
	   iptr+=NConsumed;
	   TokenNumber++;
	 }
	 else putchar(*iptr++);
       }
       else if (FType == CHENT) {
	 if (AllHTMLP){
	   if(sscanf(iptr,aHfmt,&num,&NConsumed) > 0) {
	     if(*(iptr+NConsumed-1) != ';') {
	       MicrosoftStyle++;
	       fprintf(stderr,
		       _("The HTML/HDML entity %1$s at token %2$lu of line %3$lu lacks the requisite final semicolon.\n"),
		       ExtractSubstring(tmpstr,iptr,iptr+NConsumed-3),TokenNumber,LineNo);
	       if(StrictP) {putchar(*iptr++); continue;}
	       else {putu8(num);iptr+=NConsumed;}
	     }
	     else {putu8(num);iptr+=NConsumed;}
	     TokenNumber++;
	     continue;
	   }
	   if(sscanf(iptr,aDfmt,&num,&NConsumed) > 0) {
	     if(*(iptr+NConsumed-1) != ';') {
	       MicrosoftStyle++;
	       fprintf(stderr,
		       _("The HTML/HDML entity %1$s at token %2$lu of line %3$lu lacks the requisite final semicolon.\n"),
		       ExtractSubstring(tmpstr,iptr,iptr+NConsumed-3),TokenNumber,LineNo);
	       if (StrictP) {putchar(*iptr++); continue;}
	       else {putu8(num);iptr+=NConsumed;}
	     }
	     else {putu8(num);iptr+=NConsumed;}
	     TokenNumber++;
	     continue;
	   }
	 }
	 if(sscanf(iptr,afmt,&enam,&NConsumed) > 0) {
	   if( (num = LookupCodeForEntity(enam))) {
	     if(*(iptr+NConsumed-1) != ';') {
	       MicrosoftStyle++;
	       fprintf(stderr,_("The HTML/HDML entity %1$s at token %2$lu of line %3$lu lacks the requisite final semicolon.\n"),ExtractSubstring(tmpstr,iptr,iptr+NConsumed-3),TokenNumber,LineNo);
	       if(StrictP) {putchar(*iptr++);continue;}
	       else {putu8(num);iptr+=NConsumed;}
	     }
	     else {putu8(num);iptr+=NConsumed;}
	     TokenNumber++;
	   }
	   else {
	     fprintf(stderr,"ascii2uni: unknown HTML/HDML character entity \"&%s;\" at line %lu\n",
		     enam,LineNo);
	     putu8(UNI_REPLACEMENT_CHAR);
	     iptr+=NConsumed;
	     ReplacedNumber++;
	   }
	 }
	 else putchar(*iptr++);
       } /* End of Qfmt case */
       else if( (BYTEO == FType) || (BYTED == FType) || (BYTEH == FType) ) {
	 Converted=sscanf(iptr,afmt,&b1,&b2,&b3,&NConsumed);
	 /*	 fprintf(stderr,"Converted = %d\n",Converted);fflush(stderr); */
	 switch(Converted)
	   {
	   case 3:
	     num = (((b1 * 256) + b2) * 256) + b3;
	     putu8(num);iptr+=NConsumed;
	     break;
	   case 2:
	     num = (b1 * 256) + b2;
	     putu8(num);iptr+=NConsumed;
	     break;
	   case 1:
	     num = b1;
	     putu8(num);iptr+=NConsumed;
	     break;
	   case 0:
	     putchar(*iptr++);
	     break;
	   default:
	     fprintf(stderr,
		     _("The character encoded as %1$s at token %2$lu of line %3$lu is outside the Unicode range.\n\tEmitting Unicode replacement character.\n"),
		     str,TokenNumber,LineNo);
	     putu8(UNI_REPLACEMENT_CHAR);
	   } /* end switch */
	   TokenNumber++;
       }
       else if (HDML == FType) {
	 /* HDML character references */
	 /* Need to fill this in */
       }
       else {			/* Default - not BMPSplitP, Q, or byte format */
	 if((last = sscanf(iptr,afmt,&num,&NConsumed)) > 0) {
	   if(FType== HTMLX) {
	     if(*(iptr-1+NConsumed) != ';') {
	       MicrosoftStyle++;
	       fprintf(stderr,
		       "The HTML entity %1$s at token %2$lu of line %3$lu lacks the requisite final semicolon.\n",
		       ExtractSubstring(tmpstr,iptr,iptr+NConsumed-3),TokenNumber,LineNo);
	       if(StrictP) {
		 putchar(*iptr++);
		 continue;
	       }
	     }
	   }
	   else if(FType == HTMLD) {
	     if(*(iptr-1+NConsumed) != ';') {
	       MicrosoftStyle++;
	       fprintf(stderr,
		       _("The HTML entity %1$s at token %2$lu of line %3$lu lacks the requisite final semicolon.\n"),
		       ExtractSubstring(tmpstr,iptr,iptr+NConsumed-3),TokenNumber,LineNo);
	       if(StrictP) {
		 putchar(*iptr++);
		 continue;
	       }
	     }
	   }
	   if (UTF8ValueP) putchar(num);
	   else putu8(num);
	   iptr+=NConsumed;
	   TokenNumber++;
	 } /* End of if(sscanf */
	 else putchar(*iptr++);
       }
     } /* Loop over current line */
     if(AddNewlineP) putchar('\n');
   } /* Loop over input lines */
   if(lbuf) free(lbuf);

done:
   if(VerboseP) {
     if (TokenNumber == 1)  fprintf(stderr,_("%ld token converted\n"),TokenNumber);
     else fprintf(stderr,_("%ld tokens converted\n"),TokenNumber);
     if (ReplacedNumber) {
       if (ReplacedNumber == 1) fprintf(stderr,
		_("%ld token replaced with Unicode Replacement Character\n"),ReplacedNumber);
       else fprintf(stderr,_("%ld tokens replaced with Unicode Replacement Character\n"),ReplacedNumber);
     }
     if(MicrosoftStyle) {
       if(StrictP) {
	 fprintf(stderr,
	       _("%ld Microsoft-style (lacking final semi-colon) not converted\n"),MicrosoftStyle);
       }
       else {
       fprintf(stderr,
	       _("%ld Microsoft-style (lacking final semi-colon) among those converted\n"),MicrosoftStyle);
       }
     }
   }
   exit(SUCCESS);
}
