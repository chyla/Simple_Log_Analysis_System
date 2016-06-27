/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "curl_wrapper.h"

#include <boost/log/trivial.hpp>

#include "curl.h"
#include "exception/detail/curl_object_not_created_exception.h"
#include "exception/detail/curl_general_exception.h"
#include "exception/detail/slist_append_exception.h"

using namespace std;

namespace library
{

namespace curl
{

CurlWrapperPtr CurlWrapper::Create() {
  auto curl_interface = Curl::Create();
  return CurlWrapperPtr(new CurlWrapper(curl_interface));
}

CurlWrapperPtr CurlWrapper::Create(detail::CurlInterfacePtr curl_interface) {
  return CurlWrapperPtr(new CurlWrapper(curl_interface));
}

CURL* CurlWrapper::Init() {
  BOOST_LOG_TRIVIAL(debug) << "curl::CurlWrapper::CurlWrapper: Constructor call";

  auto curl_handler = curl_interface_->EasyInit();

  if (curl_handler == nullptr) {
    BOOST_LOG_TRIVIAL(error) << "curl::CurlWrapper::CurlWrapper: Curl object not created";
    throw exception::detail::CurlObjectNotCreatedException();
  }

  return curl_handler;
}

void CurlWrapper::SetOpt(CURL* curl_handler, CURLoption option, void *parameter) {
  BOOST_LOG_TRIVIAL(debug) << "curl::CurlWrapper::SetOpt: Function call";

  auto return_code = curl_interface_->EasySetOpt(curl_handler, option, parameter);

  if (return_code != CURLE_OK) {
    BOOST_LOG_TRIVIAL(error) << "curl::CurlWrapper::SetOpt: EasySetOpt fail,"
        " error code: " << return_code << ";"
        " message: " << curl_interface_->EasyStrError(return_code);
    throw exception::detail::CurlGeneralException();
  }
}

void CurlWrapper::Perform(CURL* curl_handler) {
  BOOST_LOG_TRIVIAL(debug) << "curl::CurlWrapper::Perform: Function call";

  auto return_code = curl_interface_->EasyPerform(curl_handler);

  if (return_code != CURLE_OK) {
    BOOST_LOG_TRIVIAL(error) << "curl::CurlWrapper::Perform: EasyPerform fail,"
        " error code: " << return_code << ";"
        " message: " << curl_interface_->EasyStrError(return_code);
    throw exception::detail::CurlGeneralException();
  }
}

void CurlWrapper::Cleanup(CURL* curl_handler) {
  BOOST_LOG_TRIVIAL(debug) << "curl::CurlWrapper::Cleanup: Function call";

  curl_interface_->EasyCleanup(curl_handler);
}

curl_slist* CurlWrapper::SListAppend(curl_slist *list, const std::string &text) {
  BOOST_LOG_TRIVIAL(debug) << "curl::CurlWrapper::SListAppend: Function call";

  auto new_list = curl_interface_->SListAppend(list, text.c_str());

  if (new_list == nullptr) {
    BOOST_LOG_TRIVIAL(error) << "curl::CurlWrapper::SListAppend: list == nullptr";
    throw exception::detail::SListAppendException();
  }

  return new_list;
}

void CurlWrapper::SListFreeAll(curl_slist *list) {
  BOOST_LOG_TRIVIAL(debug) << "curl::CurlWrapper::SListFreeAll: Function call";

  curl_interface_->SListFreeAll(list);
}

CurlWrapper::CurlWrapper(detail::CurlInterfacePtr curl_interface) :
curl_interface_(curl_interface) {
}

}

}

