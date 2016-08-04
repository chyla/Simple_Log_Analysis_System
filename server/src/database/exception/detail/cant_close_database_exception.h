#pragma once

#include "src/database/exception/database_exception.h"

namespace database
{

namespace exception
{

namespace detail
{

class CantCloseDatabaseException : public DatabaseException
{
 public:
  inline char const* what() const throw () override;
};


char const* CantCloseDatabaseException::what() const throw ()
{
  return "Can't close database.";
}

}

}

}
