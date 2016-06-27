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
  virtual ~CurlWrapper() = default;

  static CurlWrapperPtr Create();
  static CurlWrapperPtr Create(detail::CurlInterfacePtr curl_interface);

  CURL* Init() override;
  
  void SetOpt(CURL* curl_handler, CURLoption option, void *parameter) override;

  void Perform(CURL* curl_handler) override;

  void Cleanup(CURL* curl_handler) override;

 private:
  CurlWrapper(detail::CurlInterfacePtr curl_interface);

  detail::CurlInterfacePtr curl_interface_;
};

}

}

#endif /* LIBRARY_CURL_CURL_WRAPPER_H */
