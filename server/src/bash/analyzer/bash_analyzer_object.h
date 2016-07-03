/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef BASH_ANALYZER_OBJECT_H
#define BASH_ANALYZER_OBJECT_H

#include "src/analyzer/analyzer_object_interface.h"
#include "src/bash/domain/detail/scripts_interface.h"

#include <memory>

namespace bash
{

namespace analyzer
{

class BashAnalyzerObject;
typedef std::shared_ptr<BashAnalyzerObject> BashAnalyzerObjectPtr;

class BashAnalyzerObject : public ::analyzer::AnalyzerObjectInterface {
 public:
  virtual ~BashAnalyzerObject() = default;

  static BashAnalyzerObjectPtr Create(::bash::domain::detail::ScriptsInterfacePtr scripts_interface);

  void Analyze() override;

 private:
  BashAnalyzerObject(::bash::domain::detail::ScriptsInterfacePtr scripts_interface);

  ::bash::domain::detail::ScriptsInterfacePtr scripts_interface_;
};

}

}

#endif /* BASH_ANALYZER_OBJECT_H */
