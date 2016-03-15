#include <iostream>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/trivial.hpp>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;


int
main(int argc, char *argv[])
{
  try {
    boost::log::add_file_log(
      keywords::file_name = LOGDIR "/server.log",
      keywords::format = ( expr::stream << "<"
                                        << boost::log::trivial::severity
                                        << ">\t"
                                        << expr::smessage
			   ),
      keywords::auto_flush = true
    );
    BOOST_LOG_TRIVIAL(info) << "Server - HelloWorld";
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

