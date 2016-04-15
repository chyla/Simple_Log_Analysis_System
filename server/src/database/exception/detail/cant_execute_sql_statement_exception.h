#ifndef PATLMS_DATABASE_EXCEPTION_DETAIL_CANT_EXECUTE_SQL_STATEMENT_EXCEPTION_H
#define PATLMS_DATABASE_EXCEPTION_DETAIL_CANT_EXECUTE_SQL_STATEMENT_EXCEPTION_H

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

#endif // PATLMS_DATABASE_EXCEPTION_DETAIL_CANT_EXECUTE_SQL_STATEMENT_EXCEPTION_H
