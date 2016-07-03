/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "bash_analyzer_object.h"

namespace bash
{

namespace analyzer
{

BashAnalyzerObjectPtr BashAnalyzerObject::Create(::bash::domain::detail::ScriptsInterfacePtr scripts_interface) {
  return BashAnalyzerObjectPtr(new BashAnalyzerObject(scripts_interface));
}

void BashAnalyzerObject::Analyze() {
  scripts_interface_->CreateDailySystemStatistics();
}

BashAnalyzerObject::BashAnalyzerObject(::bash::domain::detail::ScriptsInterfacePtr scripts_interface) :
scripts_interface_(scripts_interface) {
}

}

}
