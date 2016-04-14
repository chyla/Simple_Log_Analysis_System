AUTOMAKE_OPTIONS	= subdir-objects

AM_CPPFLAGS		= $(PTHREAD_CFLAGS) @BOOST_CPPFLAGS@ -DSYSCONFDIR="\"$(sysconfdir)\"" -DLOGDIR="\"$(localstatedir)/log/\"" $(DBUS_CFLAGS) $(SQLite3_CFLAGS)

AM_LDFLAGS		= @BOOST_LDFLAGS@

check_PROGRAMS	= tests
if CAN_RUN_TESTS
tests_SOURCES	= main.cpp \
		    database/database.cpp \
		    ../src/database/database.cpp

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
			$(SQLite3_LIBS) \
			@LIBPATLMS_LIBS@
else
tests_SOURCES	= main.cpp
endif

check-local:
	./tests

