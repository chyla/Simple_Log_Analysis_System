/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "src/library/curl/exception/curl_exception.h"

namespace library
{

namespace curl
{

namespace exception
{

namespace detail
{

class CurlObjectNotCreatedException : public CurlException {
 public:
  inline char const* what() const throw () override;
};

char const* CurlObjectNotCreatedException::what() const throw () {
  return "Curl object not created";
}

}

}

}

}
