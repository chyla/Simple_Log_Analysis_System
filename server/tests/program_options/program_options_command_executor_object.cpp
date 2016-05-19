#include <gmock/gmock.h>
#include <json/json.hpp>
#include <memory>
#include <unistd.h>

#include "src/program_options/program_options_command_executor_object.h"

using namespace nlohmann;
using namespace testing;
using namespace std;
using namespace program_options;

class ProgramOptionsCommandExecutorObjectTest : public ::testing::Test {
 public:

  ProgramOptionsCommandExecutorObjectTest()
  : options(Options::Create("root",
                            "/var/run/pidfile",
                            "/var/log/logfile",
                            "127.0.0.1",
                            8123,
                            "ipv4",
                            false,
                            false,
                            "/var/lib/database_file",
                            "127.0.0.1",
                            8124)) {
  }

  void SetUp() {
    program_options_object = ProgramOptionsCommandExecutorObject::Create(options);
  }

  void TearDown() {
  }

  virtual ~ProgramOptionsCommandExecutorObjectTest() {
  }

  const Options options;
  ProgramOptionsCommandExecutorObjectPtr program_options_object;
};

TEST_F(ProgramOptionsCommandExecutorObjectTest, Execute_UnknownCommand) {
  string command = "{ \"command\" : \"random_unknown_command\" }";

  string result = program_options_object->Execute(command);

  string expected_result = "{ \"status\" : \"unknown_command\" }";
  EXPECT_EQ(expected_result, result);
}

TEST_F(ProgramOptionsCommandExecutorObjectTest, Execute_GetServerProcessPidCommand) {
  string command = "{ \"command\" : \"get_server_process_pid\" }";

  string answer = program_options_object->Execute(command);

  auto j = json::parse(answer);
  string status = j["status"];
  long result = j["result"];

  EXPECT_EQ(status, string("ok"));
  EXPECT_EQ(result, getpid());
}

TEST_F(ProgramOptionsCommandExecutorObjectTest, Execute_GetServerProcessPidfileCommand) {
  string command = "{ \"command\" : \"get_server_process_pidfile\" }";

  string answer = program_options_object->Execute(command);

  auto j = json::parse(answer);
  string status = j["status"];
  string result = j["result"];

  EXPECT_EQ(status, string("ok"));
  EXPECT_EQ(result, options.GetPidfilePath());
}
