/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef LIBRARY_CURL_SLIST_GUARD_H
#define LIBRARY_CURL_SLIST_GUARD_H

#include "detail/curl_wrapper_interface.h"
#include "curl_wrapper.h"

#include <boost/log/trivial.hpp>

namespace library
{

namespace curl
{

class SListGuard {
 public:

  SListGuard(curl_slist *list) :
  list_(list) {
    curl_wrapper_ = CurlWrapper::Create();
  }

  SListGuard(curl_slist *list,
             detail::CurlWrapperInterfacePtr curl_wrapper) :
  list_(list),
  curl_wrapper_(curl_wrapper) {
  }

  virtual ~SListGuard() {
    BOOST_LOG_TRIVIAL(debug) << "library::curl::~SListGuard: Function call";
    curl_wrapper_->SListFreeAll(list_);
  }

 private:
  curl_slist *list_;
  detail::CurlWrapperInterfacePtr curl_wrapper_;
};

}

}


#endif /* SLIST_GUARD_H */
