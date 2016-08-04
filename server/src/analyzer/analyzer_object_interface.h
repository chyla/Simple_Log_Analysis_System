#pragma once

#include <memory>

namespace analyzer
{

class AnalyzerObjectInterface {
 public:
  virtual ~AnalyzerObjectInterface() = default;

  virtual void Analyze() = 0;
};

typedef std::shared_ptr<AnalyzerObjectInterface> AnalyzerObjectInterfacePtr;

}
