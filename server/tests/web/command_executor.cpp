#include <gmock/gmock.h>
#include <memory>

#include "tests/mock/web/simple_command_object.h"

#include "src/web/command_executor.h"

using namespace testing;
using namespace std;
using namespace web;

class CommandExecutorTest : public ::testing::Test {
 public:

  void SetUp() {
    command_executor = CommandExecutor::Create();
    simple_command_object = make_shared<mock::web::SimpleCommandObject>();
  }

  void TearDown() {
  }

  virtual ~CommandExecutorTest() {
  }

  CommandExecutorPtr command_executor;
  std::shared_ptr<mock::web::SimpleCommandObject> simple_command_object;
};

TEST_F(CommandExecutorTest, Execute_UnknownCommand) {
  string command = "{ \"command\" : \"random_unknown_command\" }";
  string result = command_executor->Execute(command);

  string expected_result = "{ \"status\" : \"unknown_command\" }";
  EXPECT_EQ(expected_result, result);
}

TEST_F(CommandExecutorTest, Execute_SimpleCommand) {
  string command = "{ \"command\" : \"simple_command\" }";
  string expected_result = "{ \"status\" : \"ok\", \"result\" : \"\" }";

  EXPECT_CALL(*simple_command_object, Execute(StrEq(command))).WillOnce(Return(expected_result));
  EXPECT_CALL(*simple_command_object, IsCommandSupported(StrEq("simple_command"))).WillOnce(Return(true));

  command_executor->RegisterCommandObject(simple_command_object);

  string result = command_executor->Execute(command);

  EXPECT_EQ(expected_result, result);
}

TEST_F(CommandExecutorTest, Execute_SimpleCommand_WhenRegisterobjectCalledTwoTimes) {
  string command = "{ \"command\" : \"simple_command\" }";
  string expected_result = "{ \"status\" : \"ok\", \"result\" : \"\" }";

  EXPECT_CALL(*simple_command_object, Execute(StrEq(command))).WillOnce(Return(expected_result));
  EXPECT_CALL(*simple_command_object, IsCommandSupported(StrEq("simple_command"))).WillOnce(Return(true));

  command_executor->RegisterCommandObject(simple_command_object);

  command_executor->RegisterCommandObject(simple_command_object);

  string result = command_executor->Execute(command);

  EXPECT_EQ(expected_result, result);
}
