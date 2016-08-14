AUTOMAKE_OPTIONS	= subdir-objects

AM_CPPFLAGS		= $(PTHREAD_CFLAGS) @BOOST_CPPFLAGS@ -DSYSCONFFILE="\"$(appconffile)\"" $(DBUS_CFLAGS)

AM_LDFLAGS		= @BOOST_LDFLAGS@

bin_PROGRAMS		= slas-agent
slas_agent_SOURCES	= main.cpp \
				program_options/options.cpp \
				program_options/parser.cpp \
				bash/bash_log_receiver.cpp \
				bash/detail/bash_dbus_thread_command.cpp \
				bash/detail/bash_proxy.cpp \
				dbus/dbus_thread.cpp \
				dbus/dbus_thread_command.cpp \
				dbus/detail/system.cpp \
				dbus/detail/dbus_thread_interface.cpp \
				apache/apache_log_receiver.cpp \
				apache/detail/apache_proxy.cpp \
				apache/detail/apache_dbus_thread_command.cpp

slas_agent_LDADD	= \
				@LIBSLAS_LIBS@ \
				@BOOST_PROGRAM_OPTIONS_LIB@ \
				@BOOST_LOG_LIB@ \
				@BOOST_LOG_SETUP_LIB@ \
				@BOOST_REGEX_LIB@ \
				@BOOST_DATE_TIME_LIB@ \
				@BOOST_FILESYSTEM_LIB@ \
				@BOOST_SYSTEM_LIB@ \
				@BOOST_THREAD_LIB@ \
				@PTHREAD_LIBS@ \
				@PTHREAD_CFLAGS@ \
				$(DBUS_LIBS)
