#include <iostream>
#include <boost/log/trivial.hpp>
#include <memory>
#include <signal.h>
#include <iostream>
#include <thread>

#include <patlms/dbus/bus.h>
#include <patlms/type/exception/exception.h>
#include <patlms/util/demonize.h>
#include <patlms/util/configure_logger.h>
#include <patlms/util/create_pidfile.h>
#include <patlms/util/remove_file.h>

#include "program_options/parser.h"
#include "database/database.h"
#include "web/command_receiver.h"
#include "web/command_executor.h"
#include "program_options/program_options_command_executor_object.h"

#include "apache/web/command_executor_object.h"
#include "objects/apache.h"
#include "objects/bash.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

dbus::BusPtr bus;
web::CommandReceiverPtr command_receiver;
std::thread command_receiver_thread;

void sigterm_handler(int sig) {
  bus->StopLoop();
  command_receiver->StopListen();
}

database::DatabasePtr CreateDatabase(const program_options::Options &options) {
  database::DatabasePtr database = database::Database::Create();
  database->Open(options.GetDatabasefilePath());
  database->CreateBashLogsTable();
  database->CreateApacheLogsTable();
  database->CreateApacheSessionTable();
  database->CreateDateTable();
  database->CreateApacheSessionExistsTable();

  return database;
}

int
main(int argc, char *argv[]) {
  program_options::Parser p;
  program_options::Options options;
  objects::BashPtr bash_object;
  objects::ApachePtr apache_object;
  database::DatabasePtr database;

  try {
    p.SetCommandLineOptions(argc, argv);
    p.SetConfigFilePath(SYSCONFFILE);
    options = p.Parse();

    if (options.IsShowHelpMessage()) {
      std::cout << "Usage: " << argv[0] << " " << "[options]\n";
      std::cout << p.GetHelpMessage() << '\n';
      return 0;
    }
  }
  catch (interface::Exception &ex) {
    std::cerr << "Error: Wrong option: " << ex.what() << '\n';
    return 1;
  }
  catch (std::exception &ex) {
    std::cerr << "Error: Missing or wrong program option.\n\n";
    std::cout << "Usage: " << argv[0] << " " << "[options]\n";
    std::cerr << p.GetHelpMessage() << "\n";
    return 1;
  }

  try {
    util::ConfigureLogger(options.GetLogfilePath());
  }
  catch (std::exception &ex) {
    std::cerr << "Error: Can't configure logger: " << ex.what() << '\n';
    return 1;
  }

  try {
    BOOST_LOG_TRIVIAL(info) << "Server";
#ifdef HAVE_CONFIG_H
    BOOST_LOG_TRIVIAL(info) << "Version: " << VERSION;
#endif

    util::Demonize(options.IsDaemon());

    database = CreateDatabase(options);

    auto options_command_object = program_options::ProgramOptionsCommandExecutorObject::Create(options);
    auto command_executor = web::CommandExecutor::Create();
    auto apache_web_command_executor = apache::web::CommandExecutorObject::Create(database);
    command_executor->RegisterCommandObject(options_command_object);
    command_executor->RegisterCommandObject(apache_web_command_executor);
    command_receiver = web::CommandReceiver::Create(command_executor);
    command_receiver->OpenPort(options.GetWebAddress(), options.GetWebPort());

    bus = std::make_shared<dbus::Bus>(dbus::Bus::Options(options.GetDbusAddress(),
                                                         options.GetDbusPort(),
                                                         options.GetDbusFamily()));
    bus->Connect();
    bus->RequestConnectionName("org.chyla.patlms.server");

    bash_object = std::make_shared<objects::Bash>(database);
    bus->RegisterObject(bash_object);

    apache_object = std::make_shared<objects::Apache>(database);
    bus->RegisterObject(apache_object);

    util::CreatePidFile(options.GetPidfilePath());

    signal(SIGTERM, sigterm_handler);
    signal(SIGINT, sigterm_handler);
    signal(SIGKILL, sigterm_handler);
    signal(SIGQUIT, sigterm_handler);

    command_receiver_thread = std::thread([]() {
      command_receiver->StartListen();
    });

    bus->Loop();
  }
  catch (std::exception &ex) {
    BOOST_LOG_TRIVIAL(fatal) << ex.what();
    return 1;
  }

  try {
    if (command_receiver)
      command_receiver->StopListen();

    if (bus)
      bus->Disconnect();

    if (bash_object)
      bash_object->FlushCache();

    if (apache_object)
      apache_object->FlushCache();

    if (database)
      database->Close();

    if (command_receiver_thread.joinable())
      command_receiver_thread.join();

    if (command_receiver)
      command_receiver->ClosePort();

    util::RemoveFile(options.GetPidfilePath());
  }
  catch (std::exception &ex) {
    BOOST_LOG_TRIVIAL(fatal) << ex.what();
    return 1;
  }

  return 0;
}

