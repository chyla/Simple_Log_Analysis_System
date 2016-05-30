#include "analyzer.h"

#include <unistd.h>

using namespace std;

namespace analyzer
{

AnalyzerPtr Analyzer::Create() {
  auto ptr = AnalyzerPtr(new Analyzer());
  return ptr;
}

void Analyzer::StartLoop() {
  is_loop_running_ = true;
  while (is_loop_running_) {
    ExecuteAll();
    sleep(1);
  }
}

void Analyzer::StopLoop() {
  is_loop_running_ = false;
}

bool Analyzer::IsLoopRunning() const {
  return is_loop_running_;
}

void Analyzer::StartAnalyzing() {
  is_analyzing_ = true;
}

bool Analyzer::IsAnalyzing() const {
  return is_analyzing_;
}

void Analyzer::AddObject(AnalyzerObjectInterfacePtr object) {
  objects_.insert(object);
}

Analyzer::Analyzer() :
is_loop_running_(false),
is_analyzing_(false) {
}

void Analyzer::ExecuteAll() {
  if (is_analyzing_) {
    for (auto obj : objects_) {
      obj->Analyze();
    }

    is_analyzing_ = false;
  }
}

}
