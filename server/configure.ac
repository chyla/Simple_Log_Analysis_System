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
	  CFLAGS='-O0 -g -Wall'
	  CXXFLAGS='-O0 -g -Wall'
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

if test "x$CUSTOM_DBUS_BINDIR" = "xno"; then
	AC_CHECK_PROG(DBUS_DAEMON_CHECK, dbus-daemon, [yes], [no])
	if test "x$DBUS_DAEMON_CHECK" != "xyes" ; then
	    AC_MSG_ERROR([cannot find dbus-daemon])
	fi
else
	AC_CHECK_PROG(DBUS_DAEMON_CHECK, dbus-daemon, [yes], [no], [$CUSTOM_DBUS_BINDIR_PATH])
	if test "x$DBUS_DAEMON_CHECK" != "xyes" ; then
		AC_MSG_ERROR([cannot find dbus-daemon in custom path])
	fi
fi

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

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# Variables
AC_SUBST(apprundir, "${localstatedir}/run/${PACKAGE}/")
AC_SUBST(appscriptdir, "${libexecdir}/${PACKAGE}/")
AC_SUBST(appconfdir, "${sysconfdir}/${PACKAGE}/")
AC_SUBST(appconffile, "${appconfdir}/server.config")
AC_SUBST(dbusconffile, "${appconfdir}/dbus.config")
AC_SUBST(dbusconftemplatefile, "${appconfdir}/dbus.config.template")

AC_OUTPUT(Makefile src/Makefile tests/Makefile configs/Makefile scripts/Makefile scripts/init/Makefile)

