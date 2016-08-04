/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "detail/curl_wrapper_interface.h"
#include "curl_wrapper.h"

#include <boost/log/trivial.hpp>

namespace library
{

namespace curl
{

class CurlGuard {
 public:

  CurlGuard(CURL *curl_handler) :
  curl_handler_(curl_handler) {
    curl_wrapper_ = CurlWrapper::Create();
  }

  CurlGuard(CURL *curl_handler,
            detail::CurlWrapperInterfacePtr curl_wrapper) :
  curl_handler_(curl_handler),
  curl_wrapper_(curl_wrapper) {
  }

  virtual ~CurlGuard() {
    BOOST_LOG_TRIVIAL(debug) << "library::curl::~CurlGuard: Function call";
    curl_wrapper_->Cleanup(curl_handler_);
  }

 private:
  CURL *curl_handler_;
  detail::CurlWrapperInterfacePtr curl_wrapper_;
};

}

}
