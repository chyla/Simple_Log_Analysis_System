#include "analyzer.h"

#include <boost/log/trivial.hpp>
#include <unistd.h>

using namespace std;

namespace analyzer
{

AnalyzerPtr Analyzer::Create() {
  BOOST_LOG_TRIVIAL(debug) << "analyzer::Analyzer::Create: Function call";
  auto ptr = AnalyzerPtr(new Analyzer());
  return ptr;
}

void Analyzer::StartLoop() {
  BOOST_LOG_TRIVIAL(debug) << "analyzer::Analyzer::StartLoop: Function call";

  is_loop_running_ = true;
  while (is_loop_running_) {
    BOOST_LOG_TRIVIAL(debug) << "analyzer::Analyzer::StartLoop: Loop is running";
    ExecuteAll();
    sleep(1);
  }

  BOOST_LOG_TRIVIAL(debug) << "analyzer::Analyzer::StartLoop: Done";
}

void Analyzer::StopLoop() {
  BOOST_LOG_TRIVIAL(debug) << "analyzer::Analyzer::StopLoop: Function call";
  is_loop_running_ = false;
}

bool Analyzer::IsLoopRunning() const {
  return is_loop_running_;
}

void Analyzer::StartAnalyzing() {
  BOOST_LOG_TRIVIAL(debug) << "analyzer::Analyzer::StartAnalyzing: Function call";
  is_analyzing_ = true;
}

bool Analyzer::IsAnalyzing() const {
  return is_analyzing_;
}

void Analyzer::AddObject(AnalyzerObjectInterfacePtr object) {
  BOOST_LOG_TRIVIAL(debug) << "analyzer::Analyzer::StartAnalyzing: Function call";
  objects_.insert(object);
}

Analyzer::Analyzer() :
is_loop_running_(false),
is_analyzing_(false) {
}

void Analyzer::ExecuteAll() {
  BOOST_LOG_TRIVIAL(debug) << "analyzer::Analyzer::ExecuteAll: Function call";
  if (is_analyzing_) {
    BOOST_LOG_TRIVIAL(info) << "analyzer::Analyzer::ExecuteAll: Analyzing started";
    for (auto obj : objects_) {
      obj->Analyze();
    }
    BOOST_LOG_TRIVIAL(info) << "analyzer::Analyzer::ExecuteAll: Analyzing finished";

    is_analyzing_ = false;
  }
}

}
