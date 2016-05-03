#include <iostream>
#include <thread>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include "program_options/parser.h"

#include <patlms/dbus/bus.h>

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

    boost::log::add_common_attributes();
    boost::log::add_file_log(
                             keywords::file_name = options.GetLogfilePath(),
                             keywords::format = (expr::stream << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%d.%m.%Y %H:%M:%S")
                                                 << ": <"
                                                 << boost::log::trivial::severity
                                                 << ">\t"
                                                 << expr::smessage
                                                 ),
                             keywords::auto_flush = true
                             );
    BOOST_LOG_TRIVIAL(info) << "Agent";
#ifdef HAVE_CONFIG_H
    BOOST_LOG_TRIVIAL(info) << "Version: " << VERSION;
#endif

    dbus::Bus::Options dbus_options(options.GetDbusAddress(),
                                    options.GetDbusPort(),
                                    options.GetDbusFamily());
    std::shared_ptr<dbus::Bus> bus = std::make_shared<dbus::Bus>(dbus_options);
    bus->Connect();
    bus->RequestConnectionName("org.chyla.patlms." + options.GetAgentName());

    auto dbus_thread = dbus::DBusThread::Create(bus);

    auto bash_log_receiver = bash::BashLogReceiver::Create(bus, dbus_thread);
    bash_log_receiver->OpenSocket("/tmp/bash-mod.sock");

    auto apache_log_receiver = apache::ApacheLogReceiver::Create(bus, dbus_thread);
    apache_log_receiver->OpenSocket("/var/run/patlms-apache.sock");

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

