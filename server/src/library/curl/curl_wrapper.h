/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "detail/curl_wrapper_interface.h"

namespace library
{

namespace curl
{

class CurlWrapper;
typedef std::shared_ptr<CurlWrapper> CurlWrapperPtr;

class CurlWrapper : public detail::CurlWrapperInterface {
 public:
  virtual ~CurlWrapper() = default;

  static CurlWrapperPtr Create();
  static CurlWrapperPtr Create(detail::CurlInterfacePtr curl_interface);

  CURL* Init() override;

  void SetOpt(CURL* curl_handler, CURLoption option, void *parameter) override;

  void SetOpt(CURL* curl_handler, CURLoption option, size_t(*f)(void*, size_t, size_t, void*)) override;

  void SetOpt(CURL* curl_handler, CURLoption option, const std::string &parameter) override;

  void SetOpt(CURL* curl_handler, CURLoption option, const char *parameter) override;

  void SetOpt(CURL* curl_handler, CURLoption option, long parameter) override;

  void SetOpt(CURL* curl_handler, CURLoption option, curl_slist *parameter) override;

  void Perform(CURL* curl_handler) override;

  void Cleanup(CURL* curl_handler) override;

  curl_slist* SListAppend(curl_slist *list, const std::string &text) override;

  void SListFreeAll(curl_slist *list) override;

 private:
  CurlWrapper(detail::CurlInterfacePtr curl_interface);

  detail::CurlInterfacePtr curl_interface_;
};

}

}
