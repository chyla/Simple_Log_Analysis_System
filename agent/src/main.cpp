#include <iostream>
#include <thread>
#include <boost/log/trivial.hpp>

#include "program_options/parser.h"

#include <patlms/dbus/bus.h>
#include <patlms/util/demonize.h>
#include <patlms/util/configure_logger.h>

#include "bash/bash_log_receiver.h"
#include "apache/apache_log_receiver.h"
#include "dbus/dbus_thread.h"

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

    BOOST_LOG_TRIVIAL(info) << "Agent";
#ifdef HAVE_CONFIG_H
    BOOST_LOG_TRIVIAL(info) << "Version: " << VERSION;
#endif
    
    util::Demonize(options.IsDaemon());

    dbus::Bus::Options dbus_options(options.GetDbusAddress(),
                                    options.GetDbusPort(),
                                    options.GetDbusFamily());
    std::shared_ptr<dbus::Bus> bus = std::make_shared<dbus::Bus>(dbus_options);
    bus->Connect();
    bus->RequestConnectionName("org.chyla.patlms." + options.GetAgentName());

    auto dbus_thread = dbus::DBusThread::Create(bus);

    auto bash_log_receiver = bash::BashLogReceiver::Create(bus, dbus_thread);
    bash_log_receiver->OpenSocket(options.GetBashSocketPath());

    auto apache_log_receiver = apache::ApacheLogReceiver::Create(bus, dbus_thread);
    apache_log_receiver->OpenSocket(options.GetApacheSocketPath());

    std::thread dbus_thread_t([&dbus_thread]
    {
      dbus_thread->StartLoop();
    });

    std::thread bash_log_receiver_t([&bash_log_receiver]
    {
      bash_log_receiver->StartLoop();
    });

    std::thread apache_log_receiver_t([&apache_log_receiver]
    {
      apache_log_receiver->StartLoop();
    });

    apache_log_receiver_t.join();
    bash_log_receiver_t.join();
    dbus_thread_t.join();
  } catch (std::exception &ex) {
    std::cerr << ex.what() << '\n';
    BOOST_LOG_TRIVIAL(fatal) << ex.what();
    return 1;
  }

  return 0;
}

