#include "configure.h"

#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

namespace logger
{

void Configure(const std::string& log_file) {
  boost::log::add_common_attributes();

  boost::log::add_file_log(keywords::file_name = log_file,
                           keywords::open_mode = (std::ios::out | std::ios::app),
                           keywords::format = (expr::stream << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%d.%m.%Y %H:%M:%S")
                           << ": <"
                           << boost::log::trivial::severity
                           << ">\t"
                           << expr::smessage
                           ),
                           keywords::auto_flush = true
                           );
}

}
