AC_PREREQ(2.59)
AC_INIT(uni2ascii, 4.18, billposer@alum.mit.edu)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE

AC_ARG_ENABLE(newsummary,
[--disable-newsummary Do not use new summary incompatible with u2a.tcl.],
[case "${enableval}" in
  yes) newsummary=true ;;
  no)  newsummary=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-newsummary) ;;
esac],[newsummary=true])

AM_CONDITIONAL(NEWSUMMARY,test "$newsummary" = true)

AC_ARG_ENABLE(debugbuild,
[--enable-debugbuild. Compile for debugging.],
[case "${enableval}" in
  yes) debugbuild=true ;;
  no)  debugbuild=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-debugbuild) ;;
esac],[debugbuild=false])

AM_CONDITIONAL(DEBUGBUILD,test "$debugbuild" = true)

# Checks for programs.
AC_PROG_CC
if ${debugbuild}; then
   CFLAGS="-ggdb -g3" 
else
   CFLAGS="-g -O2" 
fi

AC_PROG_INSTALL

AC_C_CONST
AC_TYPE_SIZE_T

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h gnu/libc-version.h libintl.h locale.h regex.h stddef.h stdio.h stdlib.h string.h sys/types.h sys/stat.h type.h unistd.h wchar.h])

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([fgetln getline regcomp setlocale strtoul])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT







