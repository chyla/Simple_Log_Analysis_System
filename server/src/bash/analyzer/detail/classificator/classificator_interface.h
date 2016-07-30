/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef CLASSIFICATOR_INTERFACE_H
#define CLASSIFICATOR_INTERFACE_H

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

#endif /* CLASSIFICATOR_INTERFACE_H */
