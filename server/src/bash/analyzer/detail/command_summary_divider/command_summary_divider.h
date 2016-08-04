/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "command_summary_divider_interface.h"

namespace bash
{

namespace analyzer
{

namespace detail
{

namespace command_summary_divider
{

class CommandSummaryDivider : public CommandSummaryDividerInterface {
 public:
  virtual ~CommandSummaryDivider() = default;

  double operator()(long long value) override {
    // interval size = (2 ^ x) / 2 + 1 when x > 0
    if (value == 0)
      return 0.0;
    else if (1 <= value && value <= 2)
      return 0.1;
    else if (3 <= value && value <= 5)
      return 0.2;
    else if (6 <= value && value <= 10)
      return 0.3;
    else if (11 <= value && value <= 19)
      return 0.4;
    else if (19 <= value && value <= 35)
      return 0.5;
    else if (36 <= value && value <= 68)
      return 0.6;
    else if (69 <= value && value <= 133)
      return 0.7;
    else if (134 <= value && value <= 262)
      return 0.8;
    else if (263 <= value && value <= 519)
      return 0.9;
    else if (520 <= value)
      return 1.0;
  }
};

}

}

}

}
