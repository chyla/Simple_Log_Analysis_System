AUTOMAKE_OPTIONS	= subdir-objects

ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS		= $(PTHREAD_CFLAGS) @BOOST_CPPFLAGS@ $(DBUS_CFLAGS) -I${top_srcdir}/include
AM_LDFLAGS		= @BOOST_LDFLAGS@ @BOOST_LOG_LIB@ @BOOST_LOG_SETUP_LIB@ @PTHREAD_LIBS@ $(DBUS_LIBS) @BOOST_SYSTEM_LIB@

check_PROGRAMS	= tests
if CAN_RUN_TESTS
tests_SOURCES	= main.cpp \
			network/network.cpp \
			type/time.cpp \
			type/date.cpp \
			util/path.cpp \
			util/create_pidfile.cpp \
			util/remove_file.cpp

OBJECT_FILES	= ../src/network/network.o \
			../src/network/detail/network_interface.o \
			../src/network/detail/system.o \
			../src/network/detail/system_interface.o \
			../src/type/time.o \
			../src/type/date.o \
			../src/util/path.o \
			../src/util/create_pidfile.o \
			../src/util/remove_file.o

tests_LDADD	= $(OBJECT_FILES) \
			@GTEST_LIBS@ \
			@GMOCK_LIBS@
else
tests_SOURCES	= main.cpp
endif

check-local:
	./tests
