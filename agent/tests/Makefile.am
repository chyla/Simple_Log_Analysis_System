AUTOMAKE_OPTIONS	= subdir-objects

AM_CPPFLAGS		= $(PTHREAD_CFLAGS) @BOOST_CPPFLAGS@ -DSYSCONFDIR="\"$(sysconfdir)\"" -DLOGDIR="\"$(localstatedir)/log/\"" $(DBUS_CFLAGS)

AM_LDFLAGS		= @BOOST_LDFLAGS@

check_PROGRAMS	= tests
if CAN_RUN_TESTS
tests_SOURCES	= main.cpp \
			dbus/dbus_thread_test.cpp \
			bash/bash_log_receiver_test.cpp \
			../src/bash/bash_log_receiver.cpp \
			../src/bash/detail/bash_proxy.cpp \
			../src/bash/detail/bash_dbus_thread_command.cpp \
			../src/bash/detail/system_interface.cpp \
			../src/bash/detail/system.cpp \
			../src/dbus/dbus_thread.cpp \
			../src/dbus/dbus_thread_command.cpp \
			../src/dbus/detail/system.cpp \
			../src/dbus/detail/dbus_thread_interface.cpp

tests_LDADD	= @GTEST_LIBS@ \
			@GMOCK_LIBS@ \
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
			$(DBUS_LIBS) \
			@LIBPATLMS_LIBS@
else
tests_SOURCES	= main.cpp
endif

check-local:
	./tests

