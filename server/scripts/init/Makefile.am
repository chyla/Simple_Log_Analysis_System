AUTOMAKE_OPTIONS	= subdir-objects

initdir			= @sysconfdir@/init.d/
init_in_FILE		= patlms-dbus.btpl patlms-server.btpl
init_generated_FILE	= $(init_in_FILE:%.btpl=%)
init_SCRIPTS		= $(init_generated_FILE)

EXTRA_DIST		= $(init_generated_FILE)
CLEANFILES		= $(init_generated_FILE)

include $(top_srcdir)/translate_btpl.rule

