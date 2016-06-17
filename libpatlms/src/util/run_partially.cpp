/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "include/patlms/util/run_partially.h"

namespace util
{

void RunPartially(long long part_count, long long summary_count,
                  std::function<void(long long, long long) > f) {
  for (long long i = 0; i < summary_count / part_count; ++i)
    f(part_count, i * part_count);

  auto left_offset = summary_count - (summary_count % part_count);
  auto left_count = summary_count % part_count;

  if (left_count > 0)
    f(left_count, left_offset);
}

}
