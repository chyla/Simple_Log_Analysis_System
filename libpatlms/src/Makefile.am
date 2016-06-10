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
					dbus/detail/dbus.cpp \
					network/network.cpp \
					network/detail/network_interface.cpp \
					network/detail/system.cpp \
					network/detail/system_interface.cpp \
					type/time.cpp \
					type/date.cpp \
					type/timestamp.cpp \
					util/path.cpp \
					util/demonize.cpp \
					util/distance.cpp \
					util/configure_logger.cpp \
					util/create_pidfile.cpp \
					util/remove_file.cpp \
					util/detail/system.cpp

pkginclude_dbusdir = $(pkgincludedir)/dbus
pkginclude_dbus_HEADERS = \
 					$(top_srcdir)/include/patlms/dbus/bus.h \
					$(top_srcdir)/include/patlms/dbus/object.h \
					$(top_srcdir)/include/patlms/dbus/proxy_object.h

pkginclude_dbus_detaildir = $(pkgincludedir)/dbus/detail
pkginclude_dbus_detail_HEADERS = \
 					$(top_srcdir)/include/patlms/dbus/detail/dbus_error_guard.h \
					$(top_srcdir)/include/patlms/dbus/detail/bus_interface.h \
					$(top_srcdir)/include/patlms/dbus/detail/dbus.h \
					$(top_srcdir)/include/patlms/dbus/detail/dbus_interface.h

pkginclude_dbus_exceptiondir = $(pkgincludedir)/dbus/exception
pkginclude_dbus_exception_HEADERS = \
 					$(top_srcdir)/include/patlms/dbus/exception/dbus_loop_exception.h

pkginclude_typedir = $(pkgincludedir)/type
pkginclude_type_HEADERS = \
					$(top_srcdir)/include/patlms/type/time.h \
					$(top_srcdir)/include/patlms/type/date.h \
					$(top_srcdir)/include/patlms/type/timestamp.h \
					$(top_srcdir)/include/patlms/type/bash_log_entry.h \
					$(top_srcdir)/include/patlms/type/apache_log_entry.h

pkginclude_type_exceptiondir = $(pkgincludedir)/type/exception
pkginclude_type_exception_HEADERS = \
 					$(top_srcdir)/include/patlms/type/exception/exception.h

pkginclude_type_exception_detaildir = $(pkgincludedir)/type/exception/detail
pkginclude_type_exception_detail_HEADERS = \
 					$(top_srcdir)/include/patlms/type/exception/detail/wrong_path_exception.h \
 					$(top_srcdir)/include/patlms/type/exception/detail/wrong_time_value_exception.h

pkginclude_networkdir = $(pkgincludedir)/network
pkginclude_network_HEADERS = \
					$(top_srcdir)/include/patlms/network/network_message.h \
					$(top_srcdir)/include/patlms/network/network.h \
					$(top_srcdir)/include/patlms/network/connection_data.h \
					$(top_srcdir)/include/patlms/network/wait_status.h

pkginclude_network_exceptiondir = $(pkgincludedir)/network/exception
pkginclude_network_exception_HEADERS = \
					$(top_srcdir)/include/patlms/network/exception/network_exception.h

pkginclude_network_exception_detaildir = $(pkgincludedir)/network/exception/detail
pkginclude_network_exception_detail_HEADERS = \
					$(top_srcdir)/include/patlms/network/exception/detail/close_exception.h \
					$(top_srcdir)/include/patlms/network/exception/detail/connect_exception.h \
					$(top_srcdir)/include/patlms/network/exception/detail/poll_exception.h \
					$(top_srcdir)/include/patlms/network/exception/detail/bad_address_exception.h \
					$(top_srcdir)/include/patlms/network/exception/detail/recv_exception.h \
					$(top_srcdir)/include/patlms/network/exception/detail/message_too_long_exception.h \
					$(top_srcdir)/include/patlms/network/exception/detail/cant_open_socket_exception.h \
					$(top_srcdir)/include/patlms/network/exception/detail/timeout_exception.h \
					$(top_srcdir)/include/patlms/network/exception/detail/accept_exception.h

pkginclude_network_detaildir = $(pkgincludedir)/network/detail
pkginclude_network_detail_HEADERS = \
					$(top_srcdir)/include/patlms/network/detail/const_values.h \
					$(top_srcdir)/include/patlms/network/detail/network_interface.h \
					$(top_srcdir)/include/patlms/network/detail/system.h \
					$(top_srcdir)/include/patlms/network/detail/system_interface.h

pkginclude_utildir = $(pkgincludedir)/util
pkginclude_util_HEADERS = \
					$(top_srcdir)/include/patlms/util/path.h \
					$(top_srcdir)/include/patlms/util/demonize.h \
					$(top_srcdir)/include/patlms/util/configure_logger.h \
					$(top_srcdir)/include/patlms/util/create_pidfile.h \
					$(top_srcdir)/include/patlms/util/distance.h \
					$(top_srcdir)/include/patlms/util/remove_file.h

pkginclude_util_detaildir = $(pkgincludedir)/util/detail
pkginclude_util_detail_HEADERS = \
					$(top_srcdir)/include/patlms/util/detail/demonize_exception.h \
					$(top_srcdir)/include/patlms/util/detail/system_interface.h \
					$(top_srcdir)/include/patlms/util/detail/system.h \
					$(top_srcdir)/include/patlms/util/detail/cant_create_pidfile_exception.h \
					$(top_srcdir)/include/patlms/util/detail/cant_remove_file_exception.h
