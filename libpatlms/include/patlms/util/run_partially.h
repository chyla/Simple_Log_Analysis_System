/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef RUN_PARTIALLY_H
#define RUN_PARTIALLY_H

#include <functional>

namespace util
{

void RunPartially(long long part_count, long long summary_count,
                  std::function<void(long long /* part count */, long long /* offset */)> f);

}

#endif /* RUN_PARTIALLY_H */
