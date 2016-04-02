AUTOMAKE_OPTIONS	= subdir-objects

scriptdir		= $(libexecdir)/@PACKAGE@
script_in_FILE		= run_dbus.btpl
script_generated_FILE	= $(script_in_FILE:%.btpl=%)
script_SCRIPTS		= $(script_generated_FILE)

EXTRA_DIST		= $(script_generated_FILE)
DISTCLEANFILES		= $(script_generated_FILE)

include $(top_srcdir)/translate_btpl.rule

install-exec-hook:
	chmod +x run_dbus

