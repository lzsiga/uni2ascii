#!/usr/bin/env python
# Time-stamp: <2008-03-10 12:06:19 poser>
#
# Filter to read a UTF-8 Unicode file and convert every character
# not within the 7-bit ASCII range to an HTML hexadecimal numeric character entity.
# At present this does not work for codepoints outside the BMP since
# the Python Unicode library is presently limited to 16 bit values.
#
# Copyright (C) 2004 William J. Poser (billposer@alum.mit.edu)
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
# or go to the web page:  http://www.gnu.org/licenses/gpl.txt.

import sys
import codecs

Version = '1.2'

if len(sys.argv) > 1:
    if sys.argv[1] == '-v':
        sys.stdout.write("uni2html %s\n" % (Version))
        sys.stdout.write("Copyright 2004, 2005 William J. Poser\n")
        sys.stdout.write("Released under the terms of the GNU General Public License.\n")
    else:
        sys.stdout.write("This program reads UTF-8 Unicode from the standard input\n")
        sys.stdout.write("and writes on the standard output the 7-bit ASCII resulting from\n")
        sys.stdout.write("the translation of every byte above 0x7F into the equivalent\n")
        sys.stdout.write("HTML hexadecimal numeric character entity.\n")
        sys.stdout.write("So long as Python's Unicode implementation is restricted to 16 bits\nonly the Basic Multilingual Plane (Plane 0) is covered.\n")
    sys.exit(2)

infile = codecs.getreader('utf-8')(sys.stdin)

CharCnt = 0
while 1:
    try:
        ch = infile.read(1)
        if not ch:
            sys.exit(0)
        CharCnt+=1        
        c = ord(ch)
        if c > 0x7F:
            sys.stdout.write("&#x%X;" % (c))
        else:
            sys.stdout.write(ch);
    except ValueError:
        sys.stderr.write("Invalid UTF-8 encountered in input after character %d.\n" % (CharCnt))
        sys.exit(1)
    except SystemExit:
        sys.exit(0)
    except IOError:
        sys.exit(0)
    except:
        sys.stderr.write("Error reading input  after character %d.\n" % (CharCnt))
        sys.exit(2)
