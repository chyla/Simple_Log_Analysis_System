/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "src/library/curl/detail/curl_wrapper_interface.h"

#include <gmock/gmock.h>
#include <memory>

namespace mock
{

namespace library
{

namespace curl
{

class CurlWrapper;
typedef std::shared_ptr<CurlWrapper> CurlWrapperPtr;

class CurlWrapper : public ::library::curl::detail::CurlWrapperInterface {
 public:
  virtual ~CurlWrapper() = default;

  static CurlWrapperPtr Create() {
    return std::make_shared<CurlWrapper>();
  }

  MOCK_METHOD0(Init, CURL*());

  MOCK_METHOD3(SetOpt, void(CURL* curl_handler, CURLoption option, void *parameter));

  MOCK_METHOD3(SetOpt, void(CURL* curl_handler, CURLoption option, size_t(*f)(void*, size_t, size_t, void*)));

  MOCK_METHOD3(SetOpt, void(CURL* curl_handler, CURLoption option, const std::string &parameter));

  MOCK_METHOD3(SetOpt, void(CURL* curl_handler, CURLoption option, const char *parameter));

  MOCK_METHOD3(SetOpt, void(CURL* curl_handler, CURLoption option, long parameter));

  MOCK_METHOD3(SetOpt, void(CURL* curl_handler, CURLoption option, curl_slist *parameter));

  MOCK_METHOD1(Perform, void(CURL* curl_handler));

  MOCK_METHOD1(Cleanup, void(CURL* curl_handler));

  MOCK_METHOD2(SListAppend, curl_slist*(curl_slist *list, const std::string &text));

  MOCK_METHOD1(SListFreeAll, void(curl_slist *list));
};

}

}

}
