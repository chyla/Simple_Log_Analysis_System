AUTOMAKE_OPTIONS	= subdir-objects

AM_CPPFLAGS		= $(PTHREAD_CFLAGS) @BOOST_CPPFLAGS@
AM_LDFLAGS		= @BOOST_LDFLAGS@

bin_PROGRAMS		= patlms-apache-helper
patlms_apache_helper_SOURCES	= main.cpp \
				    program_options/options.cpp \
				    program_options/parse_options.cpp \
				    logger/configure.cpp
patlms_apache_helper_LDADD	= @PTHREAD_LIBS@ \
				    @BOOST_PROGRAM_OPTIONS_LIB@ \
				    @BOOST_LOG_LIB@ \
				    @BOOST_LOG_SETUP_LIB@ \
				    @BOOST_SYSTEM_LIB@ \
				    @BOOST_THREAD_LIB@ \
				    @LIBPATLMS_LIBS@
