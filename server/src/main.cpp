#include <iostream>
#include <boost/log/trivial.hpp>
#include <memory>
#include <signal.h>

#include <patlms/dbus/bus.h>
#include <patlms/util/demonize.h>
#include <patlms/util/configure_logger.h>
#include <patlms/util/create_pidfile.h>
#include <patlms/util/remove_file.h>

#include "program_options/parser.h"
#include "database/database.h"

#include "objects/apache.h"
#include "objects/bash.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

std::shared_ptr<dbus::Bus> bus;

void sigterm_handler(int sig) {
  bus->StopLoop();
}

database::DatabasePtr CreateDatabase(const program_options::Options &options) {
  database::DatabasePtr database = database::Database::Create();
  database->Open(options.GetDatabasefilePath());
  database->CreateBashLogsTable();
  database->CreateApacheLogsTable();

  return database;
}

int
main(int argc, char *argv[]) {
  try {
    program_options::Parser p;
    p.SetCommandLineOptions(argc, argv);
    p.SetConfigFilePath(SYSCONFFILE);
    program_options::Options options = p.Parse();

    util::ConfigureLogger(options.GetLogfilePath());

    BOOST_LOG_TRIVIAL(info) << "Server";
#ifdef HAVE_CONFIG_H
    BOOST_LOG_TRIVIAL(info) << "Version: " << VERSION;
#endif

    util::Demonize(options.IsDaemon());

    util::CreatePidFile(options.GetPidfilePath());

    signal(SIGTERM, sigterm_handler);
    signal(SIGINT, sigterm_handler);
    signal(SIGKILL, sigterm_handler);
    signal(SIGQUIT, sigterm_handler);

    auto database = CreateDatabase(options);
    bus = std::make_shared<dbus::Bus>(dbus::Bus::Options(options.GetDbusAddress(),
                                                         options.GetDbusPort(),
                                                         options.GetDbusFamily()));
    bus->Connect();
    bus->RequestConnectionName("org.chyla.patlms.server");

    objects::Bash bash(database);
    bus->RegisterObject(&bash);

    objects::Apache apache(database);
    bus->RegisterObject(&apache);

    bus->Loop();
    
    util::RemoveFile(options.GetPidfilePath());
  }
  catch (std::exception &ex) {
    std::cerr << ex.what() << '\n';
    BOOST_LOG_TRIVIAL(fatal) << ex.what();
    return 1;
  }

  return 0;
}

