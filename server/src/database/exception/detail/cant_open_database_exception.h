#ifndef PATLMS_DATABASE_EXCEPTION_DETAIL_CANT_OPEN_DATABASE_EXCEPTION_H
#define PATLMS_DATABASE_EXCEPTION_DETAIL_CANT_OPEN_DATABASE_EXCEPTION_H

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

#endif /* PATLMS_DATABASE_EXCEPTION_CANT_OPEN_DATABASE_EXCEPTION_H */
