#ifndef POLL_RETURN_VALUE_H
#define POLL_RETURN_VALUE_H

namespace network
{

enum class WaitStatus {
  NEW_DATA,
  TIMEOUT
};

}

#endif /* POLL_RETURN_VALUE_H */
