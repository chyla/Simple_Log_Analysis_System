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

  virtual CURL* Init() = 0;

  virtual void SetOpt(CURL* curl_handler, CURLoption option, void *parameter) = 0;

  virtual void Perform(CURL* curl_handler) = 0;

  virtual void Cleanup(CURL* curl_handler) = 0;
};

typedef std::shared_ptr<CurlWrapperInterface> CurlWrapperInterfacePtr;

}

}

}

#endif /* LIBRARY_CURL_DETAIL_CURL_WRAPPER_INTERFACE_H */
