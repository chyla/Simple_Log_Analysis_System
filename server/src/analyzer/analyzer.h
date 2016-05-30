#ifndef SRC_ANALYZER_ANALYZER_H
#define SRC_ANALYZER_ANALYZER_H

#include <memory>
#include <set>

#include "analyzer_object_interface.h"
#include "detail/analyzer_interface.h"

namespace analyzer
{

class Analyzer;
typedef std::shared_ptr<Analyzer> AnalyzerPtr;

class Analyzer : public detail::AnalyzerInterface {
 public:
  virtual ~Analyzer() = default;

  static AnalyzerPtr Create();

  void StartLoop() override;
  void StopLoop() override;
  bool IsLoopRunning() const override;

  void StartAnalyzing() override;
  bool IsAnalyzing() const override;

  void AddObject(AnalyzerObjectInterfacePtr object) override;

 private:
  Analyzer();

  void ExecuteAll();

  std::set<AnalyzerObjectInterfacePtr> objects_;
  bool is_loop_running_;
  bool is_analyzing_;
};

}

#endif /* SRC_ANALYZER_ANALYZER_H */
