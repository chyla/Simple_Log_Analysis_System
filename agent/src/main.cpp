#include <iostream>
#include <thread>
#include <boost/log/trivial.hpp>
#include <signal.h>

#include "program_options/parser.h"

#include <patlms/dbus/bus.h>
#include <patlms/util/demonize.h>
#include <patlms/util/configure_logger.h>
#include <patlms/util/create_pidfile.h>
#include <patlms/util/remove_file.h>

#include "bash/bash_log_receiver.h"
#include "apache/apache_log_receiver.h"
#include "dbus/dbus_thread.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

dbus::DBusThreadPtr dbus_thread;
std::shared_ptr<bash::BashLogReceiver> bash_log_receiver;
std::shared_ptr<apache::ApacheLogReceiver> apache_log_receiver;

void sigterm_handler(int sig) {
  apache_log_receiver->StopLoop();
  bash_log_receiver->StopLoop();
  dbus_thread->StopLoop();
}

int
main(int argc, char *argv[]) {
  try {
    program_options::Parser p;
    p.SetCommandLineOptions(argc, argv);
    p.SetConfigFilePath(SYSCONFFILE);
    program_options::Options options = p.Parse();

    util::ConfigureLogger(options.GetLogfilePath(), options.IsDebug());

    BOOST_LOG_TRIVIAL(info) << "Agent";
#ifdef HAVE_CONFIG_H
    BOOST_LOG_TRIVIAL(info) << "Version: " << VERSION;
#endif

    util::Demonize(options.IsDaemon());

    util::CreatePidFile(options.GetPidfilePath());

    auto bus = std::make_shared<dbus::Bus>(dbus::Bus::Options(options.GetDbusAddress(),
                                                              options.GetDbusPort(),
                                                              options.GetDbusFamily()));
    bus->Connect();
    bus->RequestConnectionName("org.chyla.patlms." + options.GetAgentName());

    dbus_thread = dbus::DBusThread::Create(bus);

    bash_log_receiver = bash::BashLogReceiver::Create(bus, dbus_thread);
    bash_log_receiver->SetAgentName(options.GetAgentName());
    bash_log_receiver->OpenSocket(options.GetBashSocketPath());

    apache_log_receiver = apache::ApacheLogReceiver::Create(bus, dbus_thread);
    apache_log_receiver->SetAgentName(options.GetAgentName());
    apache_log_receiver->OpenSocket(options.GetApacheSocketPath());

    signal(SIGTERM, sigterm_handler);
    signal(SIGINT, sigterm_handler);
    signal(SIGKILL, sigterm_handler);
    signal(SIGQUIT, sigterm_handler);

    std::thread dbus_thread_t([] {
      dbus_thread->StartLoop();
    });

    std::thread bash_log_receiver_t([] {
      bash_log_receiver->StartLoop();
    });

    std::thread apache_log_receiver_t([] {
      apache_log_receiver->StartLoop();
    });

    apache_log_receiver_t.join();
    bash_log_receiver_t.join();
    dbus_thread_t.join();

    apache_log_receiver->CloseSocket();
    bus->Disconnect();

    util::RemoveFile(options.GetBashSocketPath());
    util::RemoveFile(options.GetApacheSocketPath());
    util::RemoveFile(options.GetPidfilePath());
  }
  catch (std::exception &ex) {
    std::cerr << ex.what() << '\n';
    BOOST_LOG_TRIVIAL(fatal) << ex.what();
    return 1;
  }

  return 0;
}
