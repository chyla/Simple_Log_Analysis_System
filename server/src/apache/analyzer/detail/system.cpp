/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "system.h"

#include <boost/log/trivial.hpp>
#include <bits/shared_ptr.h>

namespace apache
{

namespace analyzer
{

namespace detail
{

SystemPtr System::Create() {
  return std::make_shared<System>();
}

time_t System::Time(time_t *t) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::System::Time: Function call";
  return time(t);
}

struct tm* System::LocalTime(const time_t *timep) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::System::LocalTime: Function call";
  return localtime(timep);
}

}

}

}
