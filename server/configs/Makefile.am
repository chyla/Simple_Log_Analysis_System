AUTOMAKE_OPTIONS	= subdir-objects

confdir			= $(sysconfdir)/@PACKAGE@
conf_in_FILE		= server.config.btpl dbus.config.template.btpl
conf_generated_FILE	= $(conf_in_FILE:%.btpl=%)
conf_DATA		= $(conf_generated_FILE)

EXTRA_DIST		= $(conf_generated_FILE)
CLEANFILES		= $(conf_generated_FILE)

include $(top_srcdir)/translate_btpl.rule

