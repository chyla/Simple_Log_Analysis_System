/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "fann_guard.h"
#include "fann_wrapper.h"

#include <boost/log/trivial.hpp>

namespace library
{

namespace fann
{

FannGuard::FannGuard(struct fann *ann) :
ann_(ann) {
  fann_wrapper_ = FannWrapper::Create();
}

FannGuard::FannGuard(struct fann *ann, detail::FannWrapperInterfacePtr fann_wrapper) :
ann_(ann),
fann_wrapper_(fann_wrapper) {
}

FannGuard::~FannGuard() {
  BOOST_LOG_TRIVIAL(debug) << "library::fann::FannGuard::~FannGuard: Function call";
  fann_wrapper_->Destroy(ann_);
}

}

}
