/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include <gmock/gmock.h>

#include "src/library/curl/curl_wrapper.h"
#include "tests/mock/library/curl/curl.h"
#include "src/library/curl/exception/detail/curl_object_not_created_exception.h"
#include "src/library/curl/exception/detail/curl_general_exception.h"

using namespace testing;
using namespace std;

#define EXAMPLE_CURL_HANDLE (reinterpret_cast<CURL*>(0x01))
#define EXAMPLE_CURL_OPTION (CURLOPT_URL)
#define EXAMPLE_CURL_PARAM (reinterpret_cast<void*>(0x03))
#define EXAMPLE_CURL_ERROR (CURLE_WRITE_ERROR)
#define EXAMPLE_ERROR_MESSAGE ("example error message")

class CurlWrapperTest : public ::testing::Test {
 public:
  virtual ~CurlWrapperTest() = default;

  void SetUp() override {
    curl = mock::library::curl::Curl::Create();
    curl_wrapper = library::curl::CurlWrapper::Create(curl);
  }

  void TearDown() override {
  }

  mock::library::curl::CurlPtr curl;
  library::curl::CurlWrapperPtr curl_wrapper;
};

TEST_F(CurlWrapperTest, Init) {
  EXPECT_CALL(*curl, EasyInit()).WillOnce(Return(EXAMPLE_CURL_HANDLE));
  
  auto handler = curl_wrapper->Init();
  
  EXPECT_EQ(EXAMPLE_CURL_HANDLE, handler);
}

TEST_F(CurlWrapperTest, SetOpt) {
  EXPECT_CALL(*curl, EasyInit()).WillOnce(Return(EXAMPLE_CURL_HANDLE));
  EXPECT_CALL(*curl, EasySetOpt(EXAMPLE_CURL_HANDLE, EXAMPLE_CURL_OPTION, EXAMPLE_CURL_PARAM)).WillOnce(Return(CURLE_OK));

  auto curl_wrapper = library::curl::CurlWrapper::Create(curl);
  auto handler = curl_wrapper->Init();

  curl_wrapper->SetOpt(handler, EXAMPLE_CURL_OPTION, EXAMPLE_CURL_PARAM);
}

TEST_F(CurlWrapperTest, SetOpt_WhenEasySetOptFail) {
  EXPECT_CALL(*curl, EasyInit()).WillOnce(Return(EXAMPLE_CURL_HANDLE));
  EXPECT_CALL(*curl, EasySetOpt(EXAMPLE_CURL_HANDLE, EXAMPLE_CURL_OPTION, EXAMPLE_CURL_PARAM)).WillOnce(Return(EXAMPLE_CURL_ERROR));

  auto curl_wrapper = library::curl::CurlWrapper::Create(curl);
  auto handler = curl_wrapper->Init();

  EXPECT_THROW(curl_wrapper->SetOpt(handler, EXAMPLE_CURL_OPTION, EXAMPLE_CURL_PARAM), library::curl::exception::detail::CurlGeneralException);
}

TEST_F(CurlWrapperTest, Perform) {
  EXPECT_CALL(*curl, EasyInit()).WillOnce(Return(EXAMPLE_CURL_HANDLE));
  EXPECT_CALL(*curl, EasyPerform(EXAMPLE_CURL_HANDLE)).WillOnce(Return(CURLE_OK));

  auto curl_wrapper = library::curl::CurlWrapper::Create(curl);
  auto handler = curl_wrapper->Init();

  curl_wrapper->Perform(handler);
}

TEST_F(CurlWrapperTest, Perform_WhenEasyPerformFail) {
  EXPECT_CALL(*curl, EasyInit()).WillOnce(Return(EXAMPLE_CURL_HANDLE));
  EXPECT_CALL(*curl, EasyPerform(EXAMPLE_CURL_HANDLE)).WillOnce(Return(EXAMPLE_CURL_ERROR));

  auto curl_wrapper = library::curl::CurlWrapper::Create(curl);
  auto handler = curl_wrapper->Init();

  EXPECT_THROW(curl_wrapper->Perform(handler), library::curl::exception::detail::CurlGeneralException);
}
