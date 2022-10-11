bin_PROGRAMS = uni2ascii ascii2uni
bin_SCRIPTS = u2a
dist_man_MANS = uni2ascii.1 ascii2uni.1
uni2ascii_SOURCES = endian.c enttbl.c SetFormat.c uni2ascii.c UTF8in.c putu8.c
ascii2uni_SOURCES = ascii2uni.c enttbl.c GetWord.c putu8.c SetFormat.c
noinst_HEADERS = u2a_endian.h enttbl.h exitcode.h formats.h unicode.h utf8error.h
AM_CFLAGS =
if NEWSUMMARY
AM_CFLAGS += -DNEWSUMMARY
endif
if DEBUGBUILD
AM_CFLAGS += -DDEBUGBUILD
endif
AUTOMAKE_OPTIONS = dist-zip dist-bzip2
EXTRA_DIST = TestSuiteAscii2Uni CREDITS uni2html.py ascii2uni.py u2a.tcl uni2ascii.info uni2ascii-${VERSION}.lsm
u2a:	u2a.tcl
	cp u2a.tcl u2a






