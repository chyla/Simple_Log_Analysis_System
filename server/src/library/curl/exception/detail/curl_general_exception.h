/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef LIBRARY_CURL_EXCEPTION_DETAIL_CURL_GENERAL_EXCEPTION_H
#define LIBRARY_CURL_EXCEPTION_DETAIL_CURL_GENERAL_EXCEPTION_H

#include "src/library/curl/exception/curl_exception.h"

namespace library
{

namespace curl
{

namespace exception
{

namespace detail
{

class CurlGeneralException : public CurlException {
 public:
  inline char const* what() const throw () override;
};

char const* CurlGeneralException::what() const throw () {
  return "Curl exception";
}

}

}

}

}


#endif /* LIBRARY_CURL_EXCEPTION_DETAIL_CURL_GENERAL_EXCEPTION_H */
