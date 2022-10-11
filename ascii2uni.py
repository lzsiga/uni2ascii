#!/usr/bin/env python
# Time-stamp: <2008-03-10 12:05:26 poser>
#
# This is a filter that reads 7-bit ASCII containing \u-escaped Unicode
# and converts it to UTF-8 Unicode.
#
# Copyright (C) 2005 William J. Poser (billposer@alum.mit.edu)
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

Version = '1.0'

argc = len(sys.argv)
if (argc > 1) and (sys.argv[1] == "-v"):
    sys.stdout.write("ascii2uni %s\n" % (Version))
    sys.stdout.write("Copyright 2005 William J. Poser\n")
    sys.stdout.write("Released under the terms of the GNU General Public License.\n")
    sys.exit(2)
else:
    sys.stdout.write("This program is a filter that converts from 7-bit ASCII containing\n\u-escaped Unicode (as used in Python and Tcl) and UTF-8 Unicode.\n")
    sys.stdout.write("So long as Python's Unicode implementation is restricted to 16 bits\nonly the Basic Multilingual Plane (Plane 0) is covered.\n")
    
outfile = codecs.getwriter('utf-8')(sys.stdout)
outfile.write(unicode(sys.stdin.read(),'unicode-escape'))
sys.exit(0)

