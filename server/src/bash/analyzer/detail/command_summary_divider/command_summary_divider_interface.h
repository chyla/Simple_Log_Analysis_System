/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef COMMAND_SUMMARY_DIVIDER_INTERFACE_H
#define COMMAND_SUMMARY_DIVIDER_INTERFACE_H

namespace bash
{

namespace analyzer
{

namespace detail
{

namespace command_summary_divider
{

class CommandSummaryDividerInterface {
 public:
  virtual ~CommandSummaryDividerInterface() = default;
  
  virtual double operator()(long long value) = 0; 
};

}

}

}

}


#endif /* COMMAND_SUMMARY_DIVIDER_INTERFACE_H */
