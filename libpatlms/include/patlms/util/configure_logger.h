#ifndef LOGGER_CONFIGURE_H
#define LOGGER_CONFIGURE_H

#include <string>

namespace util
{

void ConfigureLogger(const std::string &log_file, bool enable_debug = false);

}

#endif /* LOGGER_CONFIGURE_H */
