#ifndef CONNECTION_IS_ALREADY_OPEN_EXCEPTION_H
#define CONNECTION_IS_ALREADY_OPEN_EXCEPTION_H

#include "src/web/exception/web_exception.h"

namespace web
{

namespace exception
{

namespace detail
{

class ConnectionIsAlreadyOpenException : public WebException
{
 public:
  inline char const* what() const throw () override;
};


char const* ConnectionIsAlreadyOpenException::what() const throw ()
{
  return "Connection is already open.";
}


}

}

}

#endif /* CONNECTION_IS_ALREADY_OPEN_EXCEPTION_H */
