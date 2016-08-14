/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include <functional>

namespace util
{

void RunPartially(long long part_count, long long summary_count,
                  std::function<void(long long /* part count */, long long /* offset */)> f);

}
