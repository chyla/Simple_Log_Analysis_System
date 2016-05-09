#include "dbus_thread.h"

#include "detail/system.h"

using namespace std;

namespace dbus
{

DBusThreadPtr DBusThread::Create(detail::BusInterfacePtr bus) {
  detail::SystemInterfacePtr system = make_shared<detail::System>();
  DBusThreadPtr thread(new DBusThread(bus, system));
  return thread;
}

DBusThreadPtr DBusThread::Create(detail::BusInterfacePtr bus,
                                 detail::SystemInterfacePtr system) {
  DBusThreadPtr thread(new DBusThread(bus, system));
  return thread;
}

void DBusThread::AddCommand(DBusThreadCommandPtr command) {
  lock_guard<mutex> guard(commands_mutex_);

  commands_.push_back(command);
}

void DBusThread::StartLoop() {
  loop_running_ = true;

  while (loop_running_) {
    if (IsCommandAvailable()) {
      auto command = GetCommand();
      command->Execute();
    }

    system_->Sleep(1);
  }
}

void DBusThread::StopLoop() {
  loop_running_ = false;
}

bool DBusThread::IsLoopRunning() {
  return loop_running_;
}

DBusThread::DBusThread(detail::BusInterfacePtr bus,
                       detail::SystemInterfacePtr system)
: bus_(bus),
system_(system),
loop_running_(false) {
}

DBusThreadCommandPtr DBusThread::GetCommand() {
  lock_guard<mutex> guard(commands_mutex_);
  auto command = commands_.front();
  commands_.pop_front();

  return command;
}

bool DBusThread::IsCommandAvailable() {
  lock_guard<mutex> guard(commands_mutex_);
  return commands_.size();
}

}
