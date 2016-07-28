/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef LIBRARY_FANN_FANN_GUARD_H
#define LIBRARY_FANN_FANN_GUARD_H

#include "detail/fann_wrapper_interface.h"

namespace library
{

namespace fann
{

class FannGuard {
 public:
  FannGuard(struct fann *ann);
  FannGuard(struct fann *ann, detail::FannWrapperInterfacePtr fann_wrapper);
  virtual ~FannGuard();

 private:
  struct fann *ann_;
  detail::FannWrapperInterfacePtr fann_wrapper_;
};

}

}

#endif /* LIBRARY_FANN_FANN_GUARD_H */
