#include <iostream>
#include <boost/log/trivial.hpp>

#include <patlms/dbus/bus.h>
#include <patlms/util/demonize.h>
#include <patlms/util/configure_logger.h>

#include "program_options/parser.h"
#include "database/database.h"

#include "objects/apache.h"
#include "objects/bash.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

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

    database::DatabasePtr database = database::Database::Create();
    database->Open(options.GetDatabasefilePath());
    database->CreateBashLogsTable();
    database->CreateApacheLogsTable();

    dbus::Bus::Options dbus_options(options.GetDbusAddress(),
                                    options.GetDbusPort(),
                                    options.GetDbusFamily());
    dbus::Bus bus(dbus_options);
    bus.Connect();
    bus.RequestConnectionName("org.chyla.patlms.server");

    objects::Bash bash(database);
    bus.RegisterObject(&bash);

    objects::Apache apache(database);
    bus.RegisterObject(&apache);
    
    bus.Loop();

  } catch (std::exception &ex) {
    std::cerr << ex.what() << '\n';
    BOOST_LOG_TRIVIAL(fatal) << ex.what();
    return 1;
  }

  return 0;
}

