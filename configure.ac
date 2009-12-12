#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.



AC_INIT([openpivot], [0.0.1], [rpechayr@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
#AC_CONFIG_SRCDIR([src/app/main.cpp])
AC_CONFIG_HEADER([config.h])
# Checks for programs.
AC_PROG_CXX

# Checks for libraries.


# Checks for header files.
AC_PATH_LIBXML2
AC_PATH_GOOGLE_SPARSE_HASH
# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST


# Figure out where hash_map lives and also hash_fun.h (or stl_hash_fun.h).
# This also tells us what namespace hash code lives in.
AC_CXX_STL_HASH
AC_CXX_STL_HASH_FUN

AC_CXX_STL_NAMESPACE
# Checks for library functions.

AC_SUBST([AM_CXXFLAGS])
AC_SUBST([AM_LDFLAGS])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
