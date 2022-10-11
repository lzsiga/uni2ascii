/*
 * Time-stamp: <2008-03-10 12:04:46 poser>
 *
 * Copyright (C) 2003 William J. Poser.
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
 * along with this program; if not, write to the
 * Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 * or go to the web page:  http://www.gnu.org/licenses/gpl.txt.
 */

#include "endian.h"

/* Returns a value indicating the endianness of the machine */

short Get_Endianness() {
  unsigned int qp = 0x0f0d0501;	/* By definition, msb = 0f, lsb = 01 */
  char *bp;
  bp = (char *) &qp;		/* bp now points at first byte of 4 byte sequence */

  if((bp[0] == 0x01) &&	/* 1234 pattern */
     (bp[1] == 0x05) &&
     (bp[2] == 0x0d) &&
     (bp[3] == 0x0f)) return(E_LITTLE_ENDIAN);

  if((bp[3] == 0x01) &&	/* 4321 pattern */
     (bp[2] == 0x05) &&
     (bp[1] == 0x0d) &&
     (bp[0] == 0x0f)) return(E_BIG_ENDIAN);

  if((bp[2] == 0x01) &&	/* 3412 pattern */
     (bp[3] == 0x05) &&
     (bp[0] == 0x0d) &&
     (bp[1] == 0x0f)) return(E_PDP_ENDIAN);

  return(E_UNKNOWN_ENDIAN);

}
