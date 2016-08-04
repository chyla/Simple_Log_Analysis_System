#pragma once

#include "src/database/exception/database_exception.h"

namespace database
{

namespace exception
{

namespace detail
{

class CantOpenDatabaseException : public DatabaseException
{
 public:
  inline char const* what() const throw () override;
};


char const* CantOpenDatabaseException::what() const throw ()
{
  return "Can't open database.";
}

}

}

}
