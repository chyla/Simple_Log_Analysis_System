/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "curl.h"

namespace library
{

namespace curl
{

CurlPtr Curl::Create() {
  return std::make_shared<Curl>();
}

CURL* Curl::EasyInit() {
  return curl_easy_init();
}

CURLcode Curl::EasySetOpt(CURL *handle, CURLoption option, void *parameter) {
  return curl_easy_setopt(handle, option, parameter);
}

CURLcode Curl::EasySetOpt(CURL *handle, CURLoption option, size_t(*f)(void*, size_t, size_t, void*)) {
  return curl_easy_setopt(handle, option, f);
}

CURLcode Curl::EasySetOpt(CURL *handle, CURLoption option, const std::string &parameter) {
  return curl_easy_setopt(handle, option, parameter.c_str());
}

CURLcode Curl::EasySetOpt(CURL *handle, CURLoption option, const char *parameter) {
  return curl_easy_setopt(handle, option, parameter);
}

CURLcode Curl::EasySetOpt(CURL *handle, CURLoption option, long parameter) {
  return curl_easy_setopt(handle, option, parameter);
}

CURLcode Curl::EasySetOpt(CURL *handle, CURLoption option, curl_slist *parameter) {
  return curl_easy_setopt(handle, option, parameter);
}

curl_slist* Curl::SListAppend(curl_slist *list, const char *string) {
  return curl_slist_append(list, string);
}

CURLcode Curl::EasyPerform(CURL *easy_handle) {
  return curl_easy_perform(easy_handle);
}

const char* Curl::EasyStrError(CURLcode errornum) {
  return curl_easy_strerror(errornum);
}

void Curl::SListFreeAll(curl_slist *list) {
  return curl_slist_free_all(list);
}

void Curl::EasyCleanup(CURL *handle) {
  return curl_easy_cleanup(handle);
}

}

}
