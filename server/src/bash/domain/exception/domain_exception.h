/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include <slas/type/exception/exception.h>

namespace bash
{

namespace domain
{

namespace exception
{

class DomainException : public interface::Exception {
 public:
  virtual ~DomainException() = default;
};

}

}

}
