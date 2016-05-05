AUTOMAKE_OPTIONS	= subdir-objects

AM_CPPFLAGS		= $(PTHREAD_CFLAGS) @BOOST_CPPFLAGS@ -DSYSCONFDIR="\"$(sysconfdir)\"" -DLOGDIR="\"$(localstatedir)/log/\"" $(DBUS_CFLAGS)

AM_LDFLAGS		= @BOOST_LDFLAGS@

check_PROGRAMS	= tests
if CAN_RUN_TESTS
tests_SOURCES	= main.cpp \
			dbus/dbus_thread_test.cpp \
			bash/bash_log_receiver_test.cpp

OBJECT_FILES	= ../src/bash/bash_log_receiver.o \
			../src/bash/detail/bash_proxy.o \
			../src/bash/detail/bash_dbus_thread_command.o \
			../src/bash/detail/system_interface.o \
			../src/bash/detail/system.o \
			../src/dbus/dbus_thread.o \
			../src/dbus/dbus_thread_command.o \
			../src/dbus/detail/system.o \
			../src/dbus/detail/dbus_thread_interface.o

tests_LDADD	= $(OBJECT_FILES) \
			@GTEST_LIBS@ \
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

