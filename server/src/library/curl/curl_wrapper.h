/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef LIBRARY_CURL_CURL_WRAPPER_H
#define LIBRARY_CURL_CURL_WRAPPER_H

#include "detail/curl_wrapper_interface.h"

namespace library
{

namespace curl
{

class CurlWrapper;
typedef std::shared_ptr<CurlWrapper> CurlWrapperPtr;

class CurlWrapper : public detail::CurlWrapperInterface {
 public:
  virtual ~CurlWrapper();

  static CurlWrapperPtr Create();
  static CurlWrapperPtr Create(detail::CurlInterfacePtr curl_interface);

  void SetOpt(CURLoption option, void *parameter) override;

  void Perform() override;

 private:
  CurlWrapper(detail::CurlInterfacePtr curl_interface);

  detail::CurlInterfacePtr curl_interface_;
  CURL *curl_handler_;
};

}

}

#endif /* LIBRARY_CURL_CURL_WRAPPER_H */
