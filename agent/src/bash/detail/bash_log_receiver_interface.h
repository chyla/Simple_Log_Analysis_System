#ifndef PATLMS_BASH_DETAIL_BASH_LOG_RECEIVER_INTERFACE_H
#define PATLMS_BASH_DETAIL_BASH_LOG_RECEIVER_INTERFACE_H

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

#endif /* PATLMS_BASH_DETAIL_BASH_LOG_RECEIVER_INTERFACE_H */
