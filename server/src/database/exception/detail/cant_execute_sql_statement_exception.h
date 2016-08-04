#pragma once

#include "src/database/exception/database_exception.h"

namespace database
{

namespace exception
{

namespace detail
{

class CantExecuteSqlStatementException : public DatabaseException
{
 public:
  inline char const* what() const throw () override;
};


char const* CantExecuteSqlStatementException::what() const throw ()
{
  return "Can't execute statement.";
}

}

}

}
