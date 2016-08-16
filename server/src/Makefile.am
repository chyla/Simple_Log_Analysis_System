AUTOMAKE_OPTIONS	= subdir-objects

AM_CPPFLAGS		= $(PTHREAD_CFLAGS) @BOOST_CPPFLAGS@ -DSYSCONFFILE="\"$(appconffile)\"" $(DBUS_CFLAGS) $(SQLite3_CFLAGS) @LIBCURL_CPPFLAGS@ -I${top_srcdir}/third_party $(FANN_CPPFLAGS)

AM_LDFLAGS		= @BOOST_LDFLAGS@ $(DBUS_LIBS)

bin_PROGRAMS		= slas-server
slas_server_SOURCES	= main.cpp \
				analyzer/analyzer.cpp \
				apache/database/database_functions.cpp \
				apache/analyzer/apache_analyzer_object.cpp \
				apache/analyzer/detail/knn_analyzer_object.cpp \
				apache/analyzer/detail/prepare_statistics_analyzer_object.cpp \
				apache/analyzer/detail/system.cpp \
				apache/analyzer/detail/prepare_statistics/nearest_neighbours_table.cpp \
				apache/notifier/type/apache_notifier_message.cpp \
				bash/analyzer/detail/daily_user_statistics_creator.cpp \
				bash/analyzer/detail/system.cpp \
				bash/analyzer/bash_analyzer_object.cpp \
				bash/analyzer/detail/classificator/classificator.cpp \
				bash/analyzer/detail/network_trainer/network_trainer.cpp \
				bash/database/detail/raw_database_functions.cpp \
				bash/database/database_functions.cpp \
				bash/dbus/object/bash.cpp \
				bash/domain/scripts.cpp \
				bash/domain/web_scripts.cpp \
				bash/web/command_executor_object.cpp \
				bash/web/detail/get_agents_with_existing_daily_user_statistics.cpp \
				bash/web/detail/get_daily_user_statistics_for_agent.cpp \
				bash/web/detail/get_anomaly_detection_configuration.cpp \
				bash/web/detail/get_commands_statistics.cpp \
				bash/web/detail/get_commands_statistics_by_configuration_id.cpp \
				bash/web/detail/get_selected_commands_ids.cpp \
				bash/web/detail/get_unconfigured_agents.cpp \
				bash/web/detail/get_configuration_id_for_agent.cpp \
				bash/web/detail/get_daily_user_statistic_by_id.cpp \
				bash/web/detail/get_daily_user_named_commands_statistics.cpp \
				bash/web/detail/get_daily_user_statistics_from_configuration.cpp \
				bash/web/detail/remove_anomaly_detection_configuration.cpp \
				bash/web/detail/save_anomaly_detection_configuration.cpp \
				bash/web/detail/save_selected_commands.cpp \
				bash/web/detail/select_default_commands.cpp \
				bash/web/detail/set_sessions_as_anomalies.cpp \
				database/database.cpp \
				database/sqlite_wrapper.cpp \
				database/general_database_functions.cpp \
				database/detail/sqlite.cpp \
				library/curl/curl.cpp \
				library/curl/curl_wrapper.cpp \
				library/fann/fann.cpp \
				library/fann/fann_guard.cpp \
				library/fann/fann_wrapper.cpp \
				mailer/mail.cpp \
				mailer/mailer.cpp \
				notifier/notifier.cpp \
				notifier/detail/system.cpp \
				program_options/parser.cpp \
				program_options/type/options.cpp \
				program_options/web/command_executor_object.cpp \
				apache/dbus/object/apache.cpp \
				web/command_receiver.cpp \
				web/command_executor.cpp \
				apache/web/command_executor_object.cpp

slas_server_LDADD	= \
				@LIBSLAS_LIBS@ \
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
				@LIBCURL@ \
				$(FANN_LIBS)
