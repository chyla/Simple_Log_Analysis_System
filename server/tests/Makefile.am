AUTOMAKE_OPTIONS	= subdir-objects

AM_CPPFLAGS		= $(PTHREAD_CFLAGS) @BOOST_CPPFLAGS@ -DSYSCONFDIR="\"$(sysconfdir)\"" -DLOGDIR="\"$(localstatedir)/log/\"" $(DBUS_CFLAGS) $(SQLite3_CFLAGS) -I${top_srcdir}/third_party

AM_LDFLAGS		= @BOOST_LDFLAGS@

check_PROGRAMS	= tests
if CAN_RUN_TESTS
tests_SOURCES	= main.cpp \
		    analyzer/analyzer.cpp \
		    apache/database/database_functions.cpp \
		    apache/analyzer/detail/prepare_statistics/nearest_neighbours_table.cpp \
		    database/database.cpp \
		    database/sqlite_wrapper.cpp \
		    database/general_database_functions.cpp \
		    web/command_executor.cpp \
		    web/command_receiver.cpp \
		    program_options/web/command_executor_object.cpp

OBJECT_FILES	= \
		    ../src/analyzer/analyzer.o \
		    ../src/apache/database/database_functions.o \
		    ../src/apache/analyzer/detail/prepare_statistics/nearest_neighbours_table.o \
		    ../src/database/database.o \
		    ../src/database/sqlite_wrapper.o \
		    ../src/database/general_database_functions.o \
		    ../src/database/detail/sqlite.o \
		    ../src/web/command_executor.o \
		    ../src/web/command_receiver.o \
		    ../src/program_options/type/options.o \
		    ../src/program_options/web/command_executor_object.o

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
			$(SQLite3_LIBS) \
			@LIBPATLMS_LIBS@
else
tests_SOURCES	= main.cpp
endif

check-local:
	./tests

