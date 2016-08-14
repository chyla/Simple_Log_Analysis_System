AUTOMAKE_OPTIONS	= subdir-objects

ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS		= @BOOST_CPPFLAGS@ $(DBUS_CFLAGS) -I${top_srcdir}/include
AM_LDFLAGS		= @BOOST_LDFLAGS@ $(DBUS_LIBS)

lib_LTLIBRARIES = lib/libslas.la
lib_libslas_la_LDFLAGS = -version-info 0:1:0
lib_libslas_la_SOURCES  = \
					dbus/bus.cpp \
					dbus/object.cpp \
					dbus/proxy_object.cpp \
					dbus/detail/dbus_error_guard.cpp \
					dbus/detail/bus_interface.cpp \
					dbus/detail/dbus.cpp \
					dbus/detail/dbus_wrapper.cpp \
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
					util/run_partially.cpp \
					util/detail/system.cpp

pkginclude_dbusdir = $(includedir)/slas/dbus
pkginclude_dbus_HEADERS = \
 					$(top_srcdir)/include/slas/dbus/bus.h \
					$(top_srcdir)/include/slas/dbus/object.h \
					$(top_srcdir)/include/slas/dbus/proxy_object.h

pkginclude_dbus_detaildir = $(includedir)/slas/dbus/detail
pkginclude_dbus_detail_HEADERS = \
 					$(top_srcdir)/include/slas/dbus/detail/dbus_error_guard.h \
					$(top_srcdir)/include/slas/dbus/detail/bus_interface.h \
					$(top_srcdir)/include/slas/dbus/detail/dbus.h \
					$(top_srcdir)/include/slas/dbus/detail/dbus_interface.h \
					$(top_srcdir)/include/slas/dbus/detail/dbus_wrapper.h \
					$(top_srcdir)/include/slas/dbus/detail/dbus_wrapper_interface.h

pkginclude_dbus_exceptiondir = $(includedir)/slas/dbus/exception
pkginclude_dbus_exception_HEADERS = \
 					$(top_srcdir)/include/slas/dbus/exception/dbus_loop_exception.h

pkginclude_typedir = $(includedir)/slas/type
pkginclude_type_HEADERS = \
					$(top_srcdir)/include/slas/type/time.h \
					$(top_srcdir)/include/slas/type/date.h \
					$(top_srcdir)/include/slas/type/day.h \
					$(top_srcdir)/include/slas/type/timestamp.h \
					$(top_srcdir)/include/slas/type/bash_log_entry.h \
					$(top_srcdir)/include/slas/type/apache_log_entry.h

pkginclude_type_exceptiondir = $(includedir)/slas/type/exception
pkginclude_type_exception_HEADERS = \
 					$(top_srcdir)/include/slas/type/exception/exception.h

pkginclude_type_exception_detaildir = $(includedir)/slas/type/exception/detail
pkginclude_type_exception_detail_HEADERS = \
 					$(top_srcdir)/include/slas/type/exception/detail/wrong_path_exception.h \
 					$(top_srcdir)/include/slas/type/exception/detail/wrong_time_value_exception.h \
 					$(top_srcdir)/include/slas/type/exception/detail/wrong_day_of_week_exception.h

pkginclude_networkdir = $(includedir)/slas/network
pkginclude_network_HEADERS = \
					$(top_srcdir)/include/slas/network/network_message.h \
					$(top_srcdir)/include/slas/network/network.h \
					$(top_srcdir)/include/slas/network/connection_data.h \
					$(top_srcdir)/include/slas/network/wait_status.h

pkginclude_network_exceptiondir = $(includedir)/slas/network/exception
pkginclude_network_exception_HEADERS = \
					$(top_srcdir)/include/slas/network/exception/network_exception.h

pkginclude_network_exception_detaildir = $(includedir)/slas/network/exception/detail
pkginclude_network_exception_detail_HEADERS = \
					$(top_srcdir)/include/slas/network/exception/detail/close_exception.h \
					$(top_srcdir)/include/slas/network/exception/detail/connect_exception.h \
					$(top_srcdir)/include/slas/network/exception/detail/poll_exception.h \
					$(top_srcdir)/include/slas/network/exception/detail/bad_address_exception.h \
					$(top_srcdir)/include/slas/network/exception/detail/recv_exception.h \
					$(top_srcdir)/include/slas/network/exception/detail/message_too_long_exception.h \
					$(top_srcdir)/include/slas/network/exception/detail/cant_open_socket_exception.h \
					$(top_srcdir)/include/slas/network/exception/detail/timeout_exception.h \
					$(top_srcdir)/include/slas/network/exception/detail/accept_exception.h

pkginclude_network_detaildir = $(includedir)/slas/network/detail
pkginclude_network_detail_HEADERS = \
					$(top_srcdir)/include/slas/network/detail/const_values.h \
					$(top_srcdir)/include/slas/network/detail/network_interface.h \
					$(top_srcdir)/include/slas/network/detail/system.h \
					$(top_srcdir)/include/slas/network/detail/system_interface.h

pkginclude_utildir = $(includedir)/slas/util
pkginclude_util_HEADERS = \
					$(top_srcdir)/include/slas/util/path.h \
					$(top_srcdir)/include/slas/util/demonize.h \
					$(top_srcdir)/include/slas/util/configure_logger.h \
					$(top_srcdir)/include/slas/util/create_pidfile.h \
					$(top_srcdir)/include/slas/util/distance.h \
					$(top_srcdir)/include/slas/util/remove_file.h \
					$(top_srcdir)/include/slas/util/run_partially.h

pkginclude_util_detaildir = $(includedir)/slas/util/detail
pkginclude_util_detail_HEADERS = \
					$(top_srcdir)/include/slas/util/detail/demonize_exception.h \
					$(top_srcdir)/include/slas/util/detail/system_interface.h \
					$(top_srcdir)/include/slas/util/detail/system.h \
					$(top_srcdir)/include/slas/util/detail/cant_create_pidfile_exception.h \
					$(top_srcdir)/include/slas/util/detail/cant_remove_file_exception.h
