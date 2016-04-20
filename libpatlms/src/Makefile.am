AUTOMAKE_OPTIONS	= subdir-objects

ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS		= @BOOST_CPPFLAGS@ $(DBUS_CFLAGS) -I${top_srcdir}/include
AM_LDFLAGS		= @BOOST_LDFLAGS@ $(DBUS_LIBS)

lib_LTLIBRARIES = lib/libpatlms.la
lib_libpatlms_la_LDFLAGS = -version-info 0:1:0
lib_libpatlms_la_SOURCES  = \
					dbus/bus.cpp \
					dbus/object.cpp \
					dbus/proxy_object.cpp \
					dbus/detail/dbus_error_guard.cpp \
					dbus/detail/bus_interface.cpp \
					type/time.cpp

pkginclude_dbusdir = $(pkgincludedir)/dbus
pkginclude_dbus_HEADERS = \
 					$(top_srcdir)/include/patlms/dbus/bus.h \
					$(top_srcdir)/include/patlms/dbus/object.h \
					$(top_srcdir)/include/patlms/dbus/proxy_object.h

pkginclude_dbus_detaildir = $(pkgincludedir)/dbus/detail
pkginclude_dbus_detail_HEADERS = \
 					$(top_srcdir)/include/patlms/dbus/detail/dbus_error_guard.h \
					$(top_srcdir)/include/patlms/dbus/detail/bus_interface.h

pkginclude_typedir = $(pkgincludedir)/type
pkginclude_type_HEADERS = \
					$(top_srcdir)/include/patlms/type/time.h \
					$(top_srcdir)/include/patlms/type/bash_log_entry.h \
					$(top_srcdir)/include/patlms/type/apache_log_entry.h

pkginclude_type_exceptiondir = $(pkgincludedir)/type/exception
pkginclude_type_exception_HEADERS = \
 					$(top_srcdir)/include/patlms/type/exception/exception.h

pkginclude_type_exception_detaildir = $(pkgincludedir)/type/exception/detail
pkginclude_type_exception_detail_HEADERS = \
 					$(top_srcdir)/include/patlms/type/exception/detail/wrong_time_value_exception.h
