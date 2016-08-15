/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "src/bash/domain/exception/domain_exception.h"

namespace bash
{

namespace domain
{

namespace exception
{

namespace detail
{

class WrongNumberOfCommands : public DomainException {
 public:
  virtual ~WrongNumberOfCommands() = default;

  char const* what() const throw () {
    return "Wrong number of commands.";
  }
};

}

}

}

}
