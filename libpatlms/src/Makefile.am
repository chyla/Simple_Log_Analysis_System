AUTOMAKE_OPTIONS	= subdir-objects

ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS		= @BOOST_CPPFLAGS@ $(DBUS_CFLAGS)
AM_LDFLAGS		= @BOOST_LDFLAGS@ $(DBUS_LIBS)

lib_LTLIBRARIES = lib/libpatlms.la
lib_libpatlms_la_LDFLAGS = -version-info 0:1:0
lib_libpatlms_la_SOURCES  = \
					dbus/bus.cpp \
					dbus/object.cpp \
					dbus/proxy_object.cpp \
					dbus/detail/dbus_error_guard.cpp

pkginclude_dbusdir = $(pkgincludedir)/dbus
pkginclude_dbus_HEADERS = \
 					dbus/bus.h \
					dbus/object.h \
					dbus/proxy_object.h

