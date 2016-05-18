#ifndef PORT_IS_CLOSED_EXCEPTION_H
#define PORT_IS_CLOSED_EXCEPTION_H

#include "src/web/exception/web_exception.h"

namespace web
{

namespace exception
{

namespace detail
{

class PortIsClosedException : public WebException
{
 public:
  inline char const* what() const throw () override;
};


char const* PortIsClosedException::what() const throw ()
{
  return "Port is closed.";
}


}

}

}

#endif /* PORT_IS_CLOSED_EXCEPTION_H */
