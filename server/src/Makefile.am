AUTOMAKE_OPTIONS	= subdir-objects

AM_CPPFLAGS		= $(PTHREAD_CFLAGS) @BOOST_CPPFLAGS@ -DSYSCONFFILE="\"$(appconffile)\"" $(DBUS_CFLAGS) $(SQLite3_CFLAGS) @LIBCURL_CPPFLAGS@ -I${top_srcdir}/third_party

AM_LDFLAGS		= @BOOST_LDFLAGS@ $(DBUS_LIBS)

bin_PROGRAMS		= patlms-server
patlms_server_SOURCES	= main.cpp \
				analyzer/analyzer.cpp \
				apache/database/database_functions.cpp \
				apache/analyzer/apache_analyzer_object.cpp \
				apache/analyzer/detail/knn_analyzer_object.cpp \
				apache/analyzer/detail/prepare_statistics_analyzer_object.cpp \
				apache/analyzer/detail/system.cpp \
				apache/analyzer/detail/prepare_statistics/nearest_neighbours_table.cpp \
				database/database.cpp \
				database/sqlite_wrapper.cpp \
				database/general_database_functions.cpp \
				database/detail/sqlite.cpp \
				program_options/parser.cpp \
				program_options/type/options.cpp \
				program_options/web/command_executor_object.cpp \
				bash/dbus/object/bash.cpp \
				apache/dbus/object/apache.cpp \
				web/command_receiver.cpp \
				web/command_executor.cpp \
				apache/web/command_executor_object.cpp

patlms_server_LDADD	= \
				@LIBPATLMS_LIBS@ \
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
				@LIBCURL@
