AUTOMAKE_OPTIONS	= subdir-objects

ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS		= $(PTHREAD_CFLAGS) @BOOST_CPPFLAGS@ $(DBUS_CFLAGS) -I${top_srcdir}/include
AM_LDFLAGS		= @BOOST_LDFLAGS@ @BOOST_LOG_LIB@ @BOOST_LOG_SETUP_LIB@ @PTHREAD_LIBS@ $(DBUS_LIBS) @BOOST_SYSTEM_LIB@

check_PROGRAMS	= tests
if CAN_RUN_TESTS
tests_SOURCES	= main.cpp \
			../src/network/network.cpp \
			../src/network/detail/network_interface.cpp \
			../src/network/detail/system.cpp \
			../src/network/detail/system_interface.cpp \
			../src/util/path.cpp \
			network/network.cpp \
			util/path.cpp

tests_LDADD	= @GTEST_LIBS@ \
			@GMOCK_LIBS@
else
tests_SOURCES	= main.cpp
endif

check-local:
	./tests
