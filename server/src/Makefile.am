AUTOMAKE_OPTIONS	= subdir-objects

AM_CPPFLAGS		= $(PTHREAD_CFLAGS) @BOOST_CPPFLAGS@ -DSYSCONFFILE="\"$(appconffile)\"" $(DBUS_CFLAGS) $(SQLite3_CFLAGS) -I${top_srcdir}/third_party

AM_LDFLAGS		= @BOOST_LDFLAGS@ $(DBUS_LIBS)

bin_PROGRAMS		= patlms-server
patlms_server_SOURCES	= main.cpp \
				database/database.cpp \
				database/detail/sqlite.cpp \
				program_options/options.cpp \
				program_options/parser.cpp \
				objects/bash.cpp \
				objects/apache.cpp \
				web/command_receiver.cpp

patlms_server_LDADD	= @BOOST_PROGRAM_OPTIONS_LIB@ \
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
