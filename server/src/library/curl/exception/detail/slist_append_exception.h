/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef LIBRARY_CURL_EXCEPTION_DETAIL_SLIST_APPEND_EXCEPTION_H
#define LIBRARY_CURL_EXCEPTION_DETAIL_SLIST_APPEND_EXCEPTION_H

#include "src/library/curl/exception/curl_exception.h"

namespace library
{

namespace curl
{

namespace exception
{

namespace detail
{

class SListAppendException : public CurlException {
 public:
  inline char const* what() const throw () override;
};

char const* SListAppendException::what() const throw () {
  return "Can't append element to list";
}

}

}

}

}


#endif /* SLIST_APPEND_EXCEPTION_H */
