/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef MOCK_MAILER_DETAIL_CURL_H
#define MOCK_MAILER_DETAIL_CURL_H

#include "src/mailer/detail/curl_interface.h"

#include <gmock/gmock.h>

namespace mock
{

namespace mailer
{

namespace detail
{

class Curl : public ::mailer::detail::CurlInterface {
 public:
  virtual ~Curl() = default;

  MOCK_METHOD0(EasyInit, CURL*());

  MOCK_METHOD3(EasySetOpt, CURLcode(CURL *handle, CURLoption option, void *parameter));

  MOCK_METHOD2(SListAppend, curl_slist*(curl_slist *list, const char *string));

  MOCK_METHOD1(EasyPerform, CURLcode(CURL *easy_handle));

  MOCK_METHOD1(EasyStrError, const char*(CURLcode errornum));

  MOCK_METHOD1(SListFreeAll, void(curl_slist *list));

  MOCK_METHOD1(EasyCleanup, void(CURL *handle));
};

}

}

}

#endif /* MOCK_MAILER_DETAIL_CURL_H */
