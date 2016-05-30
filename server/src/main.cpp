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
#include "database/sqlite_wrapper.h"
#include "web/command_receiver.h"
#include "web/command_executor.h"
#include "program_options/web/command_executor_object.h"

#include "apache/web/command_executor_object.h"
#include "src/apache/dbus/object/apache.h"
#include "src/bash/dbus/object/bash.h"

#include "analyzer/analyzer.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

dbus::BusPtr bus;
web::CommandReceiverPtr command_receiver;
analyzer::AnalyzerPtr analyzer_worker;
std::thread command_receiver_thread;
std::thread analyzer_thread;

struct sigaction act, act_usr;

void exit_signal_handler(int sig, siginfo_t *siginfo, void *context) {
  bus->StopLoop();
  command_receiver->StopListen();
  analyzer_worker->StopLoop();
}

void analyze_signal_handler(int sig, siginfo_t *siginfo, void *context) {
  analyzer_worker->StartAnalyzing();
}

database::DatabasePtr CreateDatabase(const program_options::type::Options &options) {
  database::DatabasePtr database = database::Database::Create();
  database->Open(options.GetDatabasefilePath());
  database->CreateBashLogsTable();
  database->CreateApacheLogsTable();
  database->CreateApacheSessionTable();
  database->CreateDateTable();
  database->CreateApacheSessionExistsTable();
  database->CreateApacheAnomalyDetectionConfigurationTable();

  return database;
}

int
main(int argc, char *argv[]) {
  program_options::Parser p;
  program_options::type::Options options;
  bash::dbus::object::BashPtr bash_object;
  apache::dbus::object::ApachePtr apache_object;
  database::SQLiteWrapperPtr sqlite_wrapper;
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

    sqlite_wrapper = database::SQLiteWrapper::Create();
    sqlite_wrapper->Open(options.GetDatabasefilePath());
    database = CreateDatabase(options);

    auto options_command_object = program_options::web::CommandExecutorObject::Create(options);
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

    bash_object = std::make_shared<bash::dbus::object::Bash>(database);
    bus->RegisterObject(bash_object);

    apache_object = std::make_shared<apache::dbus::object::Apache>(database);
    bus->RegisterObject(apache_object);

    util::CreatePidFile(options.GetPidfilePath());

    memset(&act, '\0', sizeof (act));
    act.sa_sigaction = exit_signal_handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGTERM, &act, nullptr);
    sigaction(SIGINT, &act, nullptr);
    sigaction(SIGKILL, &act, nullptr);
    sigaction(SIGQUIT, &act, nullptr);

    memset(&act, '\0', sizeof (act_usr));
    act_usr.sa_sigaction = analyze_signal_handler;
    act_usr.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act_usr, nullptr);

    analyzer_worker = analyzer::Analyzer::Create();

    analyzer_thread = std::thread([]() {
      analyzer_worker->StartLoop();
    });

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
    if (analyzer_worker)
      analyzer_worker->StopLoop();

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

    if (sqlite_wrapper)
      sqlite_wrapper->Close();
    
    if (analyzer_thread.joinable())
      analyzer_thread.join();

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

