#include <slas/util/configure_logger.h>

#include <boost/log/core.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace logging = boost::log;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

namespace util
{

void ConfigureLogger(const std::string& log_file, bool enable_debug) {
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

  if (enable_debug == false) {
    logging::core::get()->set_filter(
      logging::trivial::severity > logging::trivial::debug
    );
  }
}

}
