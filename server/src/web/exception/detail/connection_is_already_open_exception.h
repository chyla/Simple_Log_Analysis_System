#pragma once

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
