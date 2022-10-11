/* Time-stamp: <2008-03-10 12:05:41 poser>
 *
 * Copyright (C) 2005-2007 William J. Poser (billposer@alum.mit.edu)
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
#include <stdlib.h>
#include "unicode.h"

struct ent {
  char *name;
  UTF32 code;
};

static struct ent EntityCodeTable[] = {
{"AElig",0x00C6},
{"Aacute",0x00C1},
{"Acirc",0x00C2},
{"Agrave",0x00C0},
{"Alpha",0x0391},
{"Aring",0x00C5},
{"Atilde",0x00C3},
{"Auml",0x00C4},
{"Beta",0x0392},
{"Ccedil",0x00C7},
{"Chi",0x03A7},
{"Dagger",0x2021},
{"Delta",0x0394},
{"ETH",0x00D0},
{"Eacute",0x00C9},
{"Ecirc",0x00CA},
{"Egrave",0x00C8},
{"Epsilon",0x0395},
{"Eta",0x0397},
{"Euml",0x00CB},
{"Gamma",0x0393},
{"Iacute",0x00CD},
{"Icirc",0x00CE},
{"Igrave",0x00CC},
{"Iota",0x0399},
{"Iuml",0x00CF},
{"Kappa",0x039A},
{"Lambda",0x039B},
{"Mu",0x039C},
{"Ntilde",0x00D1},
{"Nu",0x039D},
{"OElig",0x0152},
{"Oacute",0x00D3},
{"Ocirc",0x00D4},
{"Ograve",0x00D2},
{"Omega",0x03A9},
{"Omicron",0x039F},
{"Oslash",0x00D8},
{"Otilde",0x00D5},
{"Ouml",0x00D6},
{"Phi",0x03A6},
{"Pi",0x03A0},
{"Prime",0x2033},
{"Psi",0x03A8},
{"Rho",0x03A1},
{"Scaron",0x0160},
{"Sigma",0x03A3},
{"Thorn",0x00DE},
{"Tau",0x03A4},
{"Theta",0x0398},
{"Uacute",0x00DA},
{"Ucirc",0x00DB},
{"Ugrave",0x00D9},
{"Upsilon",0x03A5},
{"Uuml",0x00DC},
{"Xi",0x039E},
{"Yacute",0x00DD},
{"Yuml",0x0178},
{"Zeta",0x0396},
{"aacute",0x00E1},
{"acirc",0x00E2},
{"acute",0x00B4},
{"aelig",0x00E6},
{"agrave",0x00E0},
{"alpha",0x03B1},
{"amp",0x0026},
{"and",0x2227},
{"ang",0x2220},
{"aring",0x00E5},
{"asymp",0x2248},
{"atilde",0x00E3},
{"auml",0x00E4},
{"bdquo",0x201E},
{"beta",0x03B2},
{"brvbar",0x00A6},
{"bull",0x2022},
{"cap",0x2229},
{"ccedil",0x00E7},
{"cedil",0x00B8},
{"cent",0x00A2},
{"chi",0x03C7},
{"circ",0x02C6},
{"clubs",0x2663},
{"cong",0x2245},
{"copy",0x00A9},
{"crarr",0x21B5},
{"cup",0x222A},
{"curren",0x00A4},
{"dArr",0x21D3},
{"dagger",0x2020},
{"darr",0x2193},
{"deg",0x00B0},
{"delta",0x03B4},
{"diams",0x2666},
{"divide",0x00F7},
{"eacute",0x00E9},
{"ecirc",0x00EA},
{"egrave",0x00E8},
{"empty",0x2205},
{"emsp",0x2003},
{"ensp",0x2002},
{"epsilon",0x03B5},
{"equiv",0x2261},
{"eta",0x03B7},
{"eth",0x00F0},
{"euml",0x00EB},
{"euro",0x20AC},
{"exist",0x2203},
{"fnof",0x0192},
{"forall",0x2200},
{"frac12",0x00BD},
{"frac14",0x00BC},
{"frac34",0x00BE},
{"frasl",0x2044},
{"gamma",0x03B3},
{"ge",0x2265},
{"gt",0x003E},
{"hArr",0x21D4},
{"harr",0x2194},
{"hearts",0x2665},
{"hellip",0x2026},
{"iacute",0x00ED},
{"icirc",0x00EE},
{"iexcl",0x00A1},
{"igrave",0x00EC},
{"infin",0x221E},
{"int",0x222B},
{"iota",0x03B9},
{"iquest",0x00BF},
{"isin",0x2208},
{"iuml",0x00EF},
{"kappa",0x03BA},
{"lArr",0x21D0},
{"lambda",0x03BB},
{"lang",0x2329},
{"laquo",0x00AB},
{"lceil",0x2308},
{"ldquo",0x201C},
{"le",0x2264},
{"lfloor",0x230A},
{"lowast",0x2217},
{"loz",0x25CA},
{"lrm",0x200E},
{"lsaquo",0x2039},
{"lsquo",0x2018},
{"lt",0x003C},
{"macr",0x00AF},
{"mdash",0x2014},
{"micro",0x00B5},
{"middot",0x00B7},
{"minus",0x2212},
{"mu",0x03BC},
{"nabla",0x2207},
{"nbsp",0x00A0},
{"ndash",0x2013},
{"ne",0x2260},
{"ni",0x220B},
{"not",0x00AC},
{"notin",0x2209},
{"nsub",0x2284},
{"ntilde",0x00F1},
{"nu",0x03BD},
{"oacute",0x00F3},
{"ocirc",0x00F4},
{"oelig",0x0153},
{"ograve",0x00F2},
{"oline",0x203E},
{"omega",0x03C9},
{"omicron",0x03BF},
{"oplus",0x2295},
{"or",0x2228},
{"ordf",0x00AA},
{"ordm",0x00BA},
{"oslash",0x00F8},
{"otilde",0x00F5},
{"otimes",0x2297},
{"ouml",0x00F6},
{"para",0x00B6},
{"part",0x2202},
{"permil",0x2030},
{"perp",0x22A5},
{"phi",0x03C6},
{"pi",0x03C0},
{"piv",0x03D6},
{"plusmn",0x00B1},
{"pound",0x00A3},
{"prime",0x2032},
{"prod",0x220F},
{"prop",0x221D},
{"psi",0x03C8},
{"quot",0x0022},
{"rArr",0x21D2},
{"radic",0x221A},
{"rang",0x232A},
{"raquo",0x00BB},
{"rarr",0x2192},
{"rceil",0x2309},
{"rdquo",0x201D},
{"reg",0x00AE},
{"rfloor",0x230B},
{"rho",0x03C1},
{"rlm",0x200F},
{"rsaquo",0x203A},
{"rsquo",0x2019},
{"sbquo",0x201A},
{"scaron",0x0161},
{"sdot",0x22C5},
{"sect",0x00A7},
{"shy",0x00AD},
{"sigma",0x03C3},
{"sigmaf",0x03C2},
{"sim",0x223C},
{"spades",0x2660},
{"sub",0x2282},
{"sube",0x2286},
{"sum",0x2211},
{"sup",0x2283},
{"sup1",0x00B9},
{"sup2",0x00B2},
{"sup3",0x00B3},
{"supe",0x2287},
{"szlig",0x00DF},
{"tau",0x03C4},
{"there4",0x2234},
{"theta",0x03B8},
{"thetasym",0x03D1},
{"thinsp",0x2009},
{"thorn",0x00FE},
{"tilde",0x02DC},
{"times",0x00D7},
{"uArr",0x21D1},
{"uacute",0x00FA},
{"uarr",0x2191},
{"ucirc",0x00FB},
{"ugrave",0x00F9},
{"uml",0x00A8},
{"upsih",0x03D2},
{"upsilon",0x03C5},
{"uuml",0x00FC},
{"weierp",0x2118},
{"xi",0x03BE},
{"yacute",0x00FD},
{"yen",0x00A5},
{"yuml",0x00FF},
{"zeta",0x03B6},
{"zwj",0x200D},
{"zwnj",0x200C}
};

static struct ent CodeEntityTable[] = {
{"quot",0x0022},
{"amp",0x0026},
{"lt",0x003C},
{"gt",0x003E},
{"nbsp",0x00A0},
{"iexcl",0x00A1},
{"cent",0x00A2},
{"pound",0x00A3},
{"curren",0x00A4},
{"yen",0x00A5},
{"brvbar",0x00A6},
{"sect",0x00A7},
{"uml",0x00A8},
{"copy",0x00A9},
{"ordf",0x00AA},
{"laquo",0x00AB},
{"not",0x00AC},
{"shy",0x00AD},
{"reg",0x00AE},
{"macr",0x00AF},
{"deg",0x00B0},
{"plusmn",0x00B1},
{"sup2",0x00B2},
{"sup3",0x00B3},
{"acute",0x00B4},
{"micro",0x00B5},
{"para",0x00B6},
{"middot",0x00B7},
{"cedil",0x00B8},
{"sup1",0x00B9},
{"ordm",0x00BA},
{"raquo",0x00BB},
{"frac14",0x00BC},
{"frac12",0x00BD},
{"frac34",0x00BE},
{"iquest",0x00BF},
{"Agrave",0x00C0},
{"Aacute",0x00C1},
{"Acirc",0x00C2},
{"Atilde",0x00C3},
{"Auml",0x00C4},
{"Aring",0x00C5},
{"AElig",0x00C6},
{"Ccedil",0x00C7},
{"Egrave",0x00C8},
{"Eacute",0x00C9},
{"Ecirc",0x00CA},
{"Euml",0x00CB},
{"Igrave",0x00CC},
{"Iacute",0x00CD},
{"Icirc",0x00CE},
{"Iuml",0x00CF},
{"ETH",0x00D0},
{"Ntilde",0x00D1},
{"Ograve",0x00D2},
{"Oacute",0x00D3},
{"Ocirc",0x00D4},
{"Otilde",0x00D5},
{"Ouml",0x00D6},
{"times",0x00D7},
{"Oslash",0x00D8},
{"Ugrave",0x00D9},
{"Uacute",0x00DA},
{"Ucirc",0x00DB},
{"Uuml",0x00DC},
{"Yacute",0x00DD},
{"Thorn",0x00DE},
{"szlig",0x00DF},
{"agrave",0x00E0},
{"aacute",0x00E1},
{"acirc",0x00E2},
{"atilde",0x00E3},
{"auml",0x00E4},
{"aring",0x00E5},
{"aelig",0x00E6},
{"ccedil",0x00E7},
{"egrave",0x00E8},
{"eacute",0x00E9},
{"ecirc",0x00EA},
{"euml",0x00EB},
{"igrave",0x00EC},
{"iacute",0x00ED},
{"icirc",0x00EE},
{"iuml",0x00EF},
{"eth",0x00F0},
{"ntilde",0x00F1},
{"ograve",0x00F2},
{"oacute",0x00F3},
{"ocirc",0x00F4},
{"otilde",0x00F5},
{"ouml",0x00F6},
{"divide",0x00F7},
{"oslash",0x00F8},
{"ugrave",0x00F9},
{"uacute",0x00FA},
{"ucirc",0x00FB},
{"uuml",0x00FC},
{"yacute",0x00FD},
{"thorn",0x00FE},
{"yuml",0x00FF},
{"OElig",0x0152},
{"oelig",0x0153},
{"Scaron",0x0160},
{"scaron",0x0161},
{"Yuml",0x0178},
{"fnof",0x0192},
{"circ",0x02C6},
{"tilde",0x02DC},
{"Alpha",0x0391},
{"Beta",0x0392},
{"Gamma",0x0393},
{"Delta",0x0394},
{"Epsilon",0x0395},
{"Zeta",0x0396},
{"Eta",0x0397},
{"Theta",0x0398},
{"Iota",0x0399},
{"Kappa",0x039A},
{"Lambda",0x039B},
{"Mu",0x039C},
{"Nu",0x039D},
{"Xi",0x039E},
{"Omicron",0x039F},
{"Pi",0x03A0},
{"Rho",0x03A1},
{"Sigma",0x03A3},
{"Tau",0x03A4},
{"Upsilon",0x03A5},
{"Phi",0x03A6},
{"Chi",0x03A7},
{"Psi",0x03A8},
{"Omega",0x03A9},
{"alpha",0x03B1},
{"beta",0x03B2},
{"gamma",0x03B3},
{"delta",0x03B4},
{"epsilon",0x03B5},
{"zeta",0x03B6},
{"eta",0x03B7},
{"theta",0x03B8},
{"iota",0x03B9},
{"kappa",0x03BA},
{"lambda",0x03BB},
{"mu",0x03BC},
{"nu",0x03BD},
{"xi",0x03BE},
{"omicron",0x03BF},
{"pi",0x03C0},
{"rho",0x03C1},
{"sigmaf",0x03C2},
{"sigma",0x03C3},
{"tau",0x03C4},
{"upsilon",0x03C5},
{"phi",0x03C6},
{"chi",0x03C7},
{"psi",0x03C8},
{"omega",0x03C9},
{"thetasym",0x03D1},
{"upsih",0x03D2},
{"piv",0x03D6},
{"ensp",0x2002},
{"emsp",0x2003},
{"thinsp",0x2009},
{"zwnj",0x200C},
{"zwj",0x200D},
{"lrm",0x200E},
{"rlm",0x200F},
{"ndash",0x2013},
{"mdash",0x2014},
{"lsquo",0x2018},
{"rsquo",0x2019},
{"sbquo",0x201A},
{"ldquo",0x201C},
{"rdquo",0x201D},
{"bdquo",0x201E},
{"dagger",0x2020},
{"Dagger",0x2021},
{"bull",0x2022},
{"hellip",0x2026},
{"permil",0x2030},
{"prime",0x2032},
{"Prime",0x2033},
{"lsaquo",0x2039},
{"rsaquo",0x203A},
{"oline",0x203E},
{"frasl",0x2044},
{"euro",0x20AC},
{"weierp",0x2118},
{"uarr",0x2191},
{"rarr",0x2192},
{"darr",0x2193},
{"harr",0x2194},
{"crarr",0x21B5},
{"lArr",0x21D0},
{"uArr",0x21D1},
{"rArr",0x21D2},
{"dArr",0x21D3},
{"hArr",0x21D4},
{"forall",0x2200},
{"part",0x2202},
{"exist",0x2203},
{"empty",0x2205},
{"nabla",0x2207},
{"isin",0x2208},
{"notin",0x2209},
{"ni",0x220B},
{"prod",0x220F},
{"sum",0x2211},
{"minus",0x2212},
{"lowast",0x2217},
{"radic",0x221A},
{"prop",0x221D},
{"infin",0x221E},
{"ang",0x2220},
{"and",0x2227},
{"or",0x2228},
{"cap",0x2229},
{"cup",0x222A},
{"int",0x222B},
{"there4",0x2234},
{"sim",0x223C},
{"cong",0x2245},
{"asymp",0x2248},
{"ne",0x2260},
{"equiv",0x2261},
{"le",0x2264},
{"ge",0x2265},
{"sub",0x2282},
{"sup",0x2283},
{"nsub",0x2284},
{"sube",0x2286},
{"supe",0x2287},
{"oplus",0x2295},
{"otimes",0x2297},
{"perp",0x22A5},
{"sdot",0x22C5},
{"lceil",0x2308},
{"rceil",0x2309},
{"lfloor",0x230A},
{"rfloor",0x230B},
{"lang",0x2329},
{"rang",0x232A},
{"loz",0x25CA},
{"spades",0x2660},
{"clubs",0x2663},
{"hearts",0x2665},
{"diams",0x2666}
};

static struct ent HDMLEntityCodeTable[] = {
{"&gt;",0x003E},
{"&lt;",0x003C},
{"&amp;",0x0026},
{"&dol;",0x0024},
{"&nbsp;",0x00A0}
};


static int Entities = sizeof(EntityCodeTable)/sizeof(struct ent);
static int HDMLEntities = sizeof(HDMLEntityCodeTable)/sizeof(struct ent);

/*
 * Look up an HTML character entity name and return the corresponding
 *Unicode codepoint or 0x00 if not found.
 */

UTF32
LookupCodeForEntity (char *s) {
  int l;			/* Lower bound of region */
  int u;			/* Upper bound of region */
  int m;			/* Midpoint of region */
  int c;
  
  l = 0;
  u = Entities -1;

  /* Standard binary search */
  while(l <= u){
    m = (l + u) / 2;
    c =strcmp(s,EntityCodeTable[m].name);
    if (c < 0) u = m - 1;
    else if(c > 0)  l = m + 1;
    else return(EntityCodeTable[m].code);
  }
  return 0L;
}

/*
 * If there exists a character entity corresponding to a
 * Unicode codepoint, return it. If not, return a null
 * string.
 */

char *
LookupEntityForCode (UTF32 s) {
  int l;			/* Lower bound of region */
  int u;			/* Upper bound of region */
  int m;			/* Midpoint of region */
  int c;
  
  l = 0;
  u = Entities -1;

  /* Standard binary search */
  while(l <= u){
    m = (l + u) / 2;
    c = s - CodeEntityTable[m].code;
    if (c < 0) u = m - 1;
    else if(c > 0)  l = m + 1;
    else return(CodeEntityTable[m].name);
  }
  return (char *) 0;
}

char *
LookupHDMLEntityForCode (UTF32 c) {
  switch (c)
    {
    case 0x003E:
      return "&gt;";
    case 0x003C:
      return "&lt;";
    case 0x0026:
      return "&amp;";
    case 0x0024:
      return "&dol;";
    case 0xA0:
      return "&nbsp;";
    default:
      return NULL;
  }
}

/*
 * Look up an HDML character entity name and return the corresponding
 *Unicode codepoint or 0x00 if not found.
 */

UTF32
LookupCodeForHDMLEntity (char *s) {
  int l;			/* Lower bound of region */
  int u;			/* Upper bound of region */
  int m;			/* Midpoint of region */
  int c;
  
  l = 0;
  u = HDMLEntities -1;

  /* Standard binary search */
  while(l <= u){
    m = (l + u) / 2;
    c =strcmp(s,HDMLEntityCodeTable[m].name);
    if (c < 0) u = m - 1;
    else if(c > 0)  l = m + 1;
    else return(HDMLEntityCodeTable[m].code);
  }
  return 0L;
}

