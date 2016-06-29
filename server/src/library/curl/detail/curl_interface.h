/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef LIBRARY_CURL_DETAIL_CURL_INTERFACE_H
#define LIBRARY_CURL_DETAIL_CURL_INTERFACE_H

#include <memory>
#include <string>
#include <curl/curl.h>

namespace library
{

namespace curl
{

namespace detail
{

class CurlInterface {
 public:
  virtual ~CurlInterface() = default;

  virtual CURL* EasyInit() = 0;

  virtual CURLcode EasySetOpt(CURL *handle, CURLoption option, void *parameter) = 0;

  virtual CURLcode EasySetOpt(CURL *handle, CURLoption option, size_t(*f)(void*, size_t, size_t, void*)) = 0;

  virtual CURLcode EasySetOpt(CURL *handle, CURLoption option, const std::string &parameter) = 0;

  virtual CURLcode EasySetOpt(CURL *handle, CURLoption option, const char *parameter) = 0;

  virtual CURLcode EasySetOpt(CURL *handle, CURLoption option, long parameter) = 0;

  virtual CURLcode EasySetOpt(CURL *handle, CURLoption option, curl_slist *parameter) = 0;

  virtual curl_slist* SListAppend(curl_slist *list, const char *string) = 0;

  virtual CURLcode EasyPerform(CURL *easy_handle) = 0;

  virtual const char* EasyStrError(CURLcode errornum) = 0;

  virtual void SListFreeAll(curl_slist *list) = 0;

  virtual void EasyCleanup(CURL *handle) = 0;
};

typedef std::shared_ptr<CurlInterface> CurlInterfacePtr;

}

}

}

#endif /* LIBRARY_CURL_DETAIL_CURL_INTERFACE_H */
