/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef LIBRARY_CURL_DETAIL_CURL_WRAPPER_INTERFACE_H
#define LIBRARY_CURL_DETAIL_CURL_WRAPPER_INTERFACE_H

#include "curl_interface.h"

#include <memory>
#include <string>

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

  virtual void SetOpt(CURL* curl_handler, CURLoption option, size_t(*f)(void*, size_t, size_t, void*)) = 0;

  virtual void SetOpt(CURL* curl_handler, CURLoption option, const std::string &parameter) = 0;

  virtual void SetOpt(CURL* curl_handler, CURLoption option, const char *parameter) = 0;

  virtual void SetOpt(CURL* curl_handler, CURLoption option, long parameter) = 0;

  virtual void SetOpt(CURL* curl_handler, CURLoption option, curl_slist *parameter) = 0;

  virtual void Perform(CURL* curl_handler) = 0;

  virtual void Cleanup(CURL* curl_handler) = 0;

  virtual curl_slist* SListAppend(curl_slist *list, const std::string &text) = 0;

  virtual void SListFreeAll(curl_slist *list) = 0;

};

typedef std::shared_ptr<CurlWrapperInterface> CurlWrapperInterfacePtr;

}

}

}

#endif /* LIBRARY_CURL_DETAIL_CURL_WRAPPER_INTERFACE_H */
