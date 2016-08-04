#pragma once

#include <string>

namespace bash
{

namespace detail
{

class BashLogReceiverInterface {
 public:
  virtual void OpenSocket(const std::string &socket_path) = 0;
};

}

}
