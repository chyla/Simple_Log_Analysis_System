AUTOMAKE_OPTIONS	= subdir-objects

AM_CPPFLAGS		= $(PTHREAD_CFLAGS) @BOOST_CPPFLAGS@ -DSYSCONFFILE="\"$(appconffile)\"" -DLOGDIR="\"$(localstatedir)/log/\"" $(DBUS_CFLAGS)

AM_LDFLAGS		= @BOOST_LDFLAGS@

bin_PROGRAMS		= agent
agent_SOURCES		= main.cpp \
				program_options/options.cpp \
				program_options/parser.cpp \
				bash/bash_log_receiver.cpp \
				bash/detail/bash_dbus_thread_command.cpp \
				bash/detail/bash_proxy.cpp \
				bash/detail/system.cpp \
				bash/detail/system_interface.cpp \
				dbus/dbus_thread.cpp \
				dbus/dbus_thread_command.cpp \
				dbus/detail/system.cpp \
				dbus/detail/dbus_thread_interface.cpp

agent_LDADD		= @BOOST_PROGRAM_OPTIONS_LIB@ \
				@BOOST_LOG_LIB@ \
				@BOOST_LOG_SETUP_LIB@ \
				@BOOST_REGEX_LIB@ \
				@BOOST_DATE_TIME_LIB@ \
				@BOOST_FILESYSTEM_LIB@ \
				@BOOST_SYSTEM_LIB@ \
				@BOOST_THREAD_LIB@ \
				@PTHREAD_LIBS@ \
				@PTHREAD_CFLAGS@ \
				$(DBUS_LIBS) \
				@LIBPATLMS_LIBS@

