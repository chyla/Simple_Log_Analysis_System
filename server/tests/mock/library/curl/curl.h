/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef MOCK_LIBRARY_CURL_CURL_H
#define MOCK_LIBRARY_CURL_CURL_H

#include "src/library/curl/detail/curl_interface.h"

#include <gmock/gmock.h>
#include <memory>

namespace mock
{

namespace library
{

namespace curl
{

class Curl;
typedef std::shared_ptr<Curl> CurlPtr;

class Curl : public ::library::curl::detail::CurlInterface {
 public:
  virtual ~Curl() = default;

  static CurlPtr Create() {
    return std::make_shared<Curl>();
  }

  MOCK_METHOD0(EasyInit, CURL*());

  MOCK_METHOD3(EasySetOpt, CURLcode(CURL *handle, CURLoption option, void *parameter));

  MOCK_METHOD3(EasySetOpt, CURLcode(CURL *handle, CURLoption option, size_t(*f)(void*, size_t, size_t, void*)));

  MOCK_METHOD3(EasySetOpt, CURLcode(CURL *handle, CURLoption option, const std::string &parameter));

  MOCK_METHOD3(EasySetOpt, CURLcode(CURL *handle, CURLoption option, const char *parameter));

  MOCK_METHOD3(EasySetOpt, CURLcode(CURL *handle, CURLoption option, long parameter));

  MOCK_METHOD3(EasySetOpt, CURLcode(CURL *handle, CURLoption option, curl_slist *parameter));

  MOCK_METHOD2(SListAppend, curl_slist*(curl_slist *list, const char *string));

  MOCK_METHOD1(EasyPerform, CURLcode(CURL *easy_handle));

  MOCK_METHOD1(EasyStrError, const char*(CURLcode errornum));

  MOCK_METHOD1(SListFreeAll, void(curl_slist *list));

  MOCK_METHOD1(EasyCleanup, void(CURL *handle));
};

}

}

}

#endif /* MOCK_LIBRARY_CURL_CURL_H */
