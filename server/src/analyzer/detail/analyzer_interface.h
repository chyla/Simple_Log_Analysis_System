#pragma once

#include <memory>

namespace analyzer
{

namespace detail
{

class AnalyzerInterface {
 public:
  virtual ~AnalyzerInterface() = default;

  virtual void StartLoop() = 0;
  virtual void StopLoop() = 0;
  virtual bool IsLoopRunning() const = 0;

  virtual void StartAnalyzing() = 0;
  virtual bool IsAnalyzing() const = 0;

  virtual void AddObject(AnalyzerObjectInterfacePtr object) = 0;
};

typedef std::shared_ptr<AnalyzerInterface> AnalyzerInterfacePtr;

}

}
