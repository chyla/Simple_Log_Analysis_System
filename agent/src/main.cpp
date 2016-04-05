#include <iostream>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;


int
main(int argc, char *argv[])
{
  try {
    boost::log::add_common_attributes();

    boost::log::add_file_log(
      keywords::file_name = LOGDIR "/agent.log",
      keywords::format = ( expr::stream << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%d.%m.%Y %H:%M:%S")
                                        << ": <"
                                        << boost::log::trivial::severity
                                        << ">\t"
                                        << expr::smessage
			   ),
      keywords::auto_flush = true
    );
    BOOST_LOG_TRIVIAL(info) << "Agent - HelloWorld";
#ifdef HAVE_CONFIG_H
    BOOST_LOG_TRIVIAL(info) << "Version: " << VERSION;
#endif

    
  } catch (std::exception &ex) {
    std::cerr << ex.what() << '\n';
    BOOST_LOG_TRIVIAL(fatal) << ex.what();
    return 1;
  }

  return 0;
}

