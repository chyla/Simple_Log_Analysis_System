#include <memory>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <bits/unique_ptr.h>

#include "src/dbus/dbus_thread.h"
#include "src/dbus/dbus_thread_command.h"

#include "tests/mock/dbus/bus.h"
#include "tests/mock/dbus/detail/system.h"

using namespace testing;
using namespace std;

class TestCommand : public dbus::DBusThreadCommand {
 public:
  int executed_count;

  TestCommand()
    : executed_count(0) {
  }

  void Execute() override {
    executed_count++;
  }
};

class StopLoopCommand : public dbus::DBusThreadCommand {
 public:

  StopLoopCommand(dbus::DBusThread &dbus_thread)
    : thread_(dbus_thread) {
  }

  void Execute() override {
    thread_.StopLoop();
  }

 private:
  dbus::DBusThread &thread_;
};

class DBusThreadTest : public ::testing::Test {
 public:

  void SetUp() {
    bus = make_shared<mock::dbus::Bus>();
    system = make_shared<mock::dbus::detail::System>();
    thread = dbus::DBusThread::Create(bus, system);
    stop_command = make_shared<StopLoopCommand>(*thread);
  }

  void TearDown() {
  }

  virtual ~DBusThreadTest() {
  }

  shared_ptr<mock::dbus::detail::System> system;
  shared_ptr<dbus::detail::BusInterface> bus;
  shared_ptr<dbus::DBusThread> thread;
  shared_ptr<StopLoopCommand> stop_command;
};

TEST_F(DBusThreadTest, StopCommand) {
  EXPECT_CALL(*system, Usleep(100)).Times(AtLeast(1));

  thread->AddCommand(stop_command);
  thread->StartLoop();

  EXPECT_FALSE(thread->IsLoopRunning());
}

TEST_F(DBusThreadTest, StopLoopWhenNotStarted) {
  ASSERT_FALSE(thread->IsLoopRunning());

  thread->StopLoop();

  EXPECT_FALSE(thread->IsLoopRunning());
}

TEST_F(DBusThreadTest, ExecuteCommandOnce) {
  EXPECT_CALL(*system, Usleep(100)).Times(1);
  shared_ptr<TestCommand> test_command = make_shared<TestCommand>();

  thread->AddCommand(test_command);
  thread->AddCommand(stop_command);
  thread->StartLoop();

  EXPECT_EQ(test_command->executed_count, 1);
}

TEST_F(DBusThreadTest, ExecuteCommandTwice) {
  EXPECT_CALL(*system, Usleep(100)).Times(1);
  shared_ptr<TestCommand> test_command = make_shared<TestCommand>();

  thread->AddCommand(test_command);
  thread->AddCommand(test_command);
  thread->AddCommand(stop_command);
  thread->StartLoop();

  EXPECT_EQ(test_command->executed_count, 2);
}
