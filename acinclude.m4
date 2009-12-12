AC_DEFUN([AC_PATH_GOOGLE_SPARSE_HASH], [

	goglesparce_use=no
	AC_ARG_WITH(google-sparse-dir,
		AC_HELP_STRING([--with-google-sparse-dir=DIR],
			[where the root of google sparse maps is installed]),
			[
			googlesparse_use=yes
			GOOGLE_SPARSE_INCLUDES="$withval"
			])
			
	if test "x${googlesparse_use}" = "xyes"
	then
	    # extend the compiler and linker flags according to the path set
		echo includes : ${GOOGLE_SPARSE_INCLUDES}
		AM_CXXFLAGS="$AM_CXXFLAGS -I${GOOGLE_SPARSE_INCLUDES}"
		ac_save_CFLAGS="$CFLAGS"
		ac_save_CPPFLAGS="$CPPFLAGS"
	
		CPPFLAGS="$GOOGLE_SPARSE_INCLUDES $CPPFLAGS"
	  AC_CHECK_HEADERS(google/type_traits.h,,
			[ AC_MSG_ERROR([google/type_traits.h was not found or was not usable, google sparse hash headers are required ! or disable it]) ]
			)
			
  		CPPFLAGS="$ac_save_CPPFLAGS"
	fi	
	AC_SUBST(GOOGLE_SPARSE_INCLUDES)
])

AC_DEFUN([AC_PATH_LIBXML2], [
	
	libxml2_use=no
	AC_ARG_ENABLE(libxml2,
		AC_HELP_STRING([--enable-libxml2],[enable compile of libxml2 components]),
		libxml2_use=yes,libxml2_use=no)

	AC_ARG_WITH(libxml2-dir,
		AC_HELP_STRING([--with-libxml2-dir=DIR],
			[where the root of libxml2 is installed]),
			[
			libxml2_use=yes
			LIBXML2="$withval"
			LIBXML2_INCLUDES="-I$withval/include/libxml2"
			LIBXML2_LIBS="$withval/lib"
			])

	if test "$libxml2_use" = "no" ; then
		AC_MSG_RESULT([libxml2 disabled, not configuring...])
	else

		if ! test "x$LIBXML2" = "x"; then
			AC_MSG_RESULT(using libxml2 from $LIBXML2)
		fi

		dnl checking some headers first
		ac_save_CFLAGS="$CFLAGS"
		ac_save_CPPFLAGS="$CPPFLAGS"
		CFLAGS="$LIBXML2_INCLUDES $CFLAGS"
		CPPFLAGS="$LIBXML2_INCLUDES $CPPFLAGS"
		AC_CHECK_HEADERS(libxml/parser.h,,
				[ AC_MSG_ERROR([libxml/parser.h was not found or was not usable, libxml2 headers are required ! or disable it]) ]
				)

		ac_save_LIBS="$LIBS"
		LIBXML2_LIBS="-L$LIBXML2_LIBS -lxml2"
		LIBS="$LIBXML2_LIBS $LIBS"
		libxml2_works=no
		dnl find the libs name
		AC_TRY_COMPILE(	[
							#include <libxml/parser.h>
						], 
						[
							LIBXML_TEST_VERSION
						],
						libxml2_works=yes, libxml2_works=no
						)
		if test "$libxml2_works" = "no"; then
			AC_MSG_ERROR([libxml2 libraries were not found !])
		fi

		CFLAGS="$ac_save_CFLAGS"
		CPPFLAGS="$ac_save_CPPFLAGS"
		LIBS="$ac_save_LIBS"
	fi

	AM_CONDITIONAL(COMPILE_LIBXML2, test "x$libxml2_use" = "xyes")
	AC_SUBST(LIBXML2)
	AC_SUBST(LIBXML2_INCLUDES)
	AC_SUBST(LIBXML2_LIBS)

])
m4_include([m4/stl_hash.m4])
m4_include([m4/stl_hash_fun.m4])
m4_include([m4/stl_namespace.m4])
m4_include([m4/namespaces.m4])

