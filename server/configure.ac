#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
R
m4_include([m4/ax_cxx_compile_stdcxx_11.m4])
m4_include([m4/ax_pthread.m4])
m4_include([m4/ax_boost_base.m4])
m4_include([m4/ax_boost_program_options.m4])
m4_include([m4/ax_boost_unit_test_framework.m4])
m4_include([m4/ax_boost_thread.m4])
m4_include([m4/ax_boost_filesystem.m4])
m4_include([m4/ax_boost_system.m4])
m4_include([m4/ax_boost_date_time.m4])
m4_include([m4/ax_boost_regex.m4])
m4_include([m4/ax_boost_log.m4])
m4_include([m4/ax_boost_log_setup.m4])

AC_PREREQ([2.69])
AC_INIT(PATLMS, master, adam@chyla.org)
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
	  CFLAGS='-O2'
	  CXXFLAGS='-O2'
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

AX_PTHREAD([], [AC_MSG_ERROR([cannot find pthread library])])

AX_BOOST_BASE([1.55], [], [AC_MSG_ERROR([cannot find Boost libraries])])

AX_BOOST_PROGRAM_OPTIONS
if test "x$ax_cv_boost_program_options" != "xyes"; then
    AC_MSG_ERROR([cannot find the Boost Program Options library])
fi

AX_BOOST_UNIT_TEST_FRAMEWORK
AM_CONDITIONAL(HAVE_BOOST_UNIT_TEST_FRAMEWORK, [test "x$ax_cv_boost_unit_test_framework" = "xyes"]) 

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

AC_OUTPUT(Makefile src/Makefile tests/Makefile configs/Makefile scripts/Makefile scripts/init/Makefile)

