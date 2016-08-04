/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include <memory>

namespace bash
{

namespace analyzer
{

namespace detail
{

namespace classificator
{

class ClassificatorInterface {
 public:
  virtual ~ClassificatorInterface() = default;

  virtual void Analyze() = 0;
};

typedef std::shared_ptr<ClassificatorInterface> ClassificatorInterfacePtr;

}

}

}

}
