#ifndef SRC_ANALYZER_ANALYZER_OBJECT_INTERFACE_H
#define SRC_ANALYZER_ANALYZER_OBJECT_INTERFACE_H

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

#endif /* SRC_ANALYZER_ANALYZER_OBJECT_INTERFACE_H */
