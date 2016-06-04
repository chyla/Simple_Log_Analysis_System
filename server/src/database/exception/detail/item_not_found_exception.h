#ifndef PATLMS_DATABASE_EXCEPTION_DETAIL_ITEM_NOT_FOUND_EXCEPTION_H
#define PATLMS_DATABASE_EXCEPTION_DETAIL_ITEM_NOT_FOUND_EXCEPTION_H

#include "src/database/exception/database_exception.h"


namespace database
{

namespace exception
{

namespace detail
{

class ItemNotFoundException : public DatabaseException
{
 public:
  inline char const* what() const throw () override;
};


char const* ItemNotFoundException::what() const throw ()
{
  return "Item not found.";
}

}

}

}

#endif // PATLMS_DATABASE_EXCEPTION_DETAIL_ITEM_NOT_FOUND_EXCEPTION_H
