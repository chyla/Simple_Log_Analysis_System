#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

m4_include([m4/ax_cxx_compile_stdcxx_11.m4])
m4_include([m4/ax_pthread.m4])
m4_include([m4/ax_boost_base.m4])
m4_include([m4/ax_boost_system.m4])
m4_include([m4/ax_boost_log.m4])
m4_include([m4/ax_boost_log_setup.m4])

AC_PREREQ([2.69])
AC_INIT(PATLMS,
	m4_esyscmd_s(git describe --dirty --always --tags || ( echo "Not a git repository, can't check application version. You can ignore that error." 1>&2 && echo "unknown-version" )),
	adam@chyla.org)
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_ARG_ENABLE(debug,
	[AS_HELP_STRING([--enable-debug], [Enable debug mode])],
	[
	  CFLAGS='-O0 -g -Wall -pedantic'
	  CXXFLAGS='-O0 -g -Wall -pedantic'
	  AC_DEFINE([HAVE_ENABLE_DEBUG], , [define if debug mode is enabled])
	],
	[
	  CFLAGS="$CFLAGS -O2"
	  CXXFLAGS="$CXXFLAGS -O2"
	])

AC_ARG_WITH([custom-dbus-binary-path],
	AC_HELP_STRING([--with-custom-dbus-binary-path=ARG],
			[Set custom D-Bus daemon binary directory path]),
	[
	  if test "x$withval" = "xno"; then
		CUSTOM_DBUS_BINDIR="no"
	  elif test "x$withval" = "xyes"; then
		AC_MSG_ERROR(--with-boost-libdir expected path to binary)
	  else
		CUSTOM_DBUS_BINDIR="yes"
		CUSTOM_DBUS_BINDIR_PATH="$withval"
		AC_SUBST(CUSTOM_DBUS_BINDIR_PATH, $withval)
	  fi
	],
	[
	  CUSTOM_DBUS_BINDIR="no"
          CUSTOM_DBUS_BINDIR_PATH=""
	]
)

AC_LANG_CPLUSPLUS

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.
AX_CXX_COMPILE_STDCXX_11([noext])

AX_PTHREAD([
            LIBS="$LIBS $PTHREAD_LIBS"
            CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
            CXXFLAGS="$CXXFLAGS $PTHREAD_CFLAGS"
           ],
           [AC_MSG_ERROR([cannot find pthread library])]
          )

AC_MSG_CHECKING([Checking for Google Test])
save_LIBS=$LIBS
GTEST_LIBS="-lgtest"
LIBS="$LIBS $GTEST_LIBS"
AC_LINK_IFELSE([AC_LANG_SOURCE([
                    #include <gtest/gtest.h>
                    int main(int argc, char **argv) {
                        testing::InitGoogleTest(&argc, argv);
                        return 0;
                    }
                ])
               ],
               [AC_DEFINE([HAVE_GTEST], , [define if the Google Test library is available])
                AC_SUBST(GTEST_LIBS)
                AM_CONDITIONAL([HAVE_GTEST], [true])
                AC_MSG_RESULT(yes)
               ],
               [
                AM_CONDITIONAL([HAVE_GTEST], [false])
                AC_MSG_RESULT(no)
               ]
              )
LIBS=$save_LIBS

AC_MSG_CHECKING([Checking for Google Mock])
save_LIBS=$LIBS
GMOCK_LIBS="-lgmock -lgtest"
LIBS="$LIBS $GMOCK_LIBS"
AC_LINK_IFELSE([AC_LANG_SOURCE([
                    #include <gmock/gmock.h>
                    int main(int argc, char **argv) {
                        testing::InitGoogleMock(&argc, argv);
                        return 0;
                    }
                ])
               ],
               [AC_DEFINE([HAVE_GMOCK], , [define if the Google Mock library is available])
                AC_SUBST(GMOCK_LIBS)
                AM_CONDITIONAL([HAVE_GMOCK], [true])
                AC_MSG_RESULT(yes)
               ],
               [
                AM_CONDITIONAL([HAVE_GMOCK], [false])
                AC_MSG_RESULT(no)
               ]
              )
LIBS=$save_LIBS

AM_CONDITIONAL([CAN_RUN_TESTS], [test @HAVE_GMOCK@ -a @HAVE_GTEST@])

AX_BOOST_BASE([1.54], [], [AC_MSG_ERROR([cannot find Boost libraries])])

AX_BOOST_SYSTEM
if test "x$ax_cv_boost_system" != "xyes"; then
   AC_MSG_ERROR([cannot find the Boost System Library])
fi

AX_BOOST_LOG

AX_BOOST_LOG_SETUP

PKG_CHECK_MODULES([DBUS], dbus-1 >= 1.6.18)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_OUTPUT(Makefile src/Makefile tests/Makefile)

