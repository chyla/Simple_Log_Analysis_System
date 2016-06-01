/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include <patlms/util/distance.h>

#include <cstdlib>

namespace util
{

long long Distance(const ::type::Time &t1, const ::type::Time &t2) {
  long long t1_secs = t1.GetHour() * 60 * 60 + t1.GetMinute() * 60 + t1.GetSecond();
  long long t2_secs = t2.GetHour() * 60 * 60 + t2.GetMinute() * 60 + t2.GetSecond();

  return abs(t2_secs - t1_secs);
}

}
