#ifndef PATLMS_DATABASE_EXCEPTION_CANT_CLOSE_DATABASE_EXCEPTION_H
#define PATLMS_DATABASE_EXCEPTION_CANT_CLOSE_DATABASE_EXCEPTION_H

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

#endif /* PATLMS_DATABASE_EXCEPTION_CANT_CLOSE_DATABASE_EXCEPTION_H */
