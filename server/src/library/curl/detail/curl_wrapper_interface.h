/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef LIBRARY_CURL_DETAIL_CURL_WRAPPER_INTERFACE_H
#define LIBRARY_CURL_DETAIL_CURL_WRAPPER_INTERFACE_H

#include "curl_interface.h"

#include <memory>

namespace library
{

namespace curl
{

namespace detail
{

class CurlWrapperInterface {
 public:
  virtual ~CurlWrapperInterface() = default;

  virtual void SetOpt(CURLoption option, void *parameter) = 0;

  virtual void Perform() = 0;
};

typedef std::shared_ptr<CurlWrapperInterface> CurlWrapperInterfacePtr;

}

}

}

#endif /* LIBRARY_CURL_DETAIL_CURL_WRAPPER_INTERFACE_H */
