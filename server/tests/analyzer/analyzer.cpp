#include <thread>
#include <unistd.h>
#include <gmock/gmock.h>

#include "src/analyzer/analyzer.h"

#include "tests/mock/analyzer/analyzer_object.h"

using namespace testing;
using namespace std;
using namespace analyzer;

class AnalyzerTest : public ::testing::Test {
 public:
  virtual ~AnalyzerTest() = default;

  void SetUp() override {
    mock_object1 = ::mock::analyzer::AnalyzerObject::Create();
    mock_object2 = ::mock::analyzer::AnalyzerObject::Create();
    analyzer = Analyzer::Create();
  }

  void TearDown() override {
  }

  ::mock::analyzer::AnalyzerObjectPtr mock_object1;
  ::mock::analyzer::AnalyzerObjectPtr mock_object2;
  AnalyzerPtr analyzer;
};

TEST_F(AnalyzerTest, Create) {
  EXPECT_FALSE(analyzer->IsLoopRunning());
  EXPECT_FALSE(analyzer->IsAnalyzing());
}

TEST_F(AnalyzerTest, StartStopLoopAndExecuteAnalyzeWithOneObject) {
  EXPECT_CALL(*mock_object1, Analyze()).Times(1);

  ASSERT_FALSE(analyzer->IsLoopRunning());
  ASSERT_FALSE(analyzer->IsAnalyzing());

  analyzer->AddObject(mock_object1);

  thread t([this]() {
    analyzer->StartLoop();
  });

  sleep(1);

  analyzer->StartAnalyzing();

  sleep(2);

  analyzer->StopLoop();
  t.join();

  EXPECT_FALSE(analyzer->IsLoopRunning());
  EXPECT_FALSE(analyzer->IsAnalyzing());
}

TEST_F(AnalyzerTest, StartStopLoopAndExecuteAnalyzeWithTwoObjects) {
  EXPECT_CALL(*mock_object1, Analyze()).Times(1);
  EXPECT_CALL(*mock_object2, Analyze()).Times(1);

  ASSERT_FALSE(analyzer->IsLoopRunning());
  ASSERT_FALSE(analyzer->IsAnalyzing());

  analyzer->AddObject(mock_object1);
  analyzer->AddObject(mock_object2);

  thread t([this]() {
    analyzer->StartLoop();
  });

  sleep(1);

  analyzer->StartAnalyzing();

  sleep(2);

  analyzer->StopLoop();
  t.join();

  EXPECT_FALSE(analyzer->IsLoopRunning());
  EXPECT_FALSE(analyzer->IsAnalyzing());
}

TEST_F(AnalyzerTest, StartStopLoopAndAddOneObjectWithoutExecutingAnalyzing) {
  ASSERT_FALSE(analyzer->IsLoopRunning());
  ASSERT_FALSE(analyzer->IsAnalyzing());

  analyzer->AddObject(mock_object1);

  thread t([this]() {
    analyzer->StartLoop();
  });

  sleep(1);

  analyzer->StopLoop();
  t.join();

  EXPECT_FALSE(analyzer->IsLoopRunning());
  EXPECT_FALSE(analyzer->IsAnalyzing());
}
