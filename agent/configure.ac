#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

m4_include([m4/ax_cxx_compile_stdcxx_11.m4])
m4_include([m4/ax_pthread.m4])
m4_include([m4/ax_boost_base.m4])
m4_include([m4/ax_boost_program_options.m4])
m4_include([m4/ax_boost_thread.m4])
m4_include([m4/ax_boost_filesystem.m4])
m4_include([m4/ax_boost_system.m4])
m4_include([m4/ax_boost_date_time.m4])
m4_include([m4/ax_boost_regex.m4])
m4_include([m4/ax_boost_log.m4])
m4_include([m4/ax_boost_log_setup.m4])

AC_PREREQ([2.69])
AC_INIT(PATLMS,
	m4_esyscmd_s(git describe --dirty --always --tags),
	adam@chyla.org)
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])

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

AC_LANG_CPLUSPLUS

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

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

AX_BOOST_BASE([1.55], [], [AC_MSG_ERROR([cannot find Boost libraries])])

AX_BOOST_PROGRAM_OPTIONS
if test "x$ax_cv_boost_program_options" != "xyes"; then
    AC_MSG_ERROR([cannot find the Boost Program Options library])
fi

AX_BOOST_THREAD
if test "x$ax_cv_boost_thread" != "xyes"; then
   AC_MSG_ERROR([cannot find the Boost Thread Library])
fi

AX_BOOST_FILESYSTEM
if test "x$ax_cv_boost_filesystem" != "xyes"; then
   AC_MSG_ERROR([cannot find the Boost Filesystem Library])
fi

AX_BOOST_SYSTEM
if test "x$ax_cv_boost_system" != "xyes"; then
   AC_MSG_ERROR([cannot find the Boost System Library])
fi

AX_BOOST_DATE_TIME
if test "x$ax_cv_boost_date_time" != "xyes"; then
   AC_MSG_ERROR([cannot find the Boost Date_Time Library])
fi

AX_BOOST_REGEX
if test "x$ax_cv_boost_regex" != "xyes"; then
   AC_MSG_ERROR([cannot find the Boost Regex Library])
fi

AX_BOOST_LOG

AX_BOOST_LOG_SETUP

PKG_CHECK_MODULES([DBUS], dbus-1 >= 1.8.0)

AC_MSG_CHECKING([Checking for PATLMS library])
save_LIBS=$LIBS
save_CPPFLAGS=$CPPFLAGS
LIBPATLMS_LIBS="$DBUS_LIBS $BOOST_LOG_LIB -lpatlms"
LIBPATLMS_CPPFLAGS="$DBUS_CFLAGS $BOOST_CPPFLAGS"
LIBS="$LIBS $LIBPATLMS_LIBS"
CPPFLAGS="$CPPFLAGS $LIBPATLMS_CPPFLAGS"
AC_LINK_IFELSE([AC_LANG_SOURCE([
                    #include <patlms/dbus/bus.h>
                    int main(int argc, char **argv) {
                        dbus::Bus::Options opt("127.0.0.1", 1234, "ipv4");
                        dbus::Bus bus(opt);
                        return 0;
                    }
                ])
               ],
               [AC_DEFINE([HAVE_LIBPATLMS], , [define if the PATLMS library is available])
                AC_SUBST(LIBPATLMS_LIBS)
                AM_CONDITIONAL([HAVE_LIBPATLMS], [true])
                ax_patlms_library="yes"
                AC_MSG_RESULT(yes)
               ],
               [
                AM_CONDITIONAL([HAVE_LIBPATLMS], [false])
                ax_patlms_library="no"
                AC_MSG_RESULT(no)
               ]
              )
LIBS=$save_LIBS
CPPFLAGS=$save_CPPFLAGS

if test "x$ax_patlms_library" != "xyes"; then
  AC_MSG_ERROR([cannot find the PATLMS library])
fi

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# Variables
AC_SUBST(apprundir, "${localstatedir}/run/${PACKAGE}/")
AC_SUBST(appscriptdir, "${libexecdir}/${PACKAGE}/")
AC_SUBST(appconfdir, "${sysconfdir}/${PACKAGE}/")
AC_SUBST(appconffile, "${appconfdir}/agent.config")

AC_OUTPUT(Makefile src/Makefile tests/Makefile configs/Makefile)
