#include "dbus_thread.h"

#include "detail/system.h"

using namespace std;

namespace dbus
{

DBusThread::DBusThreadPtr DBusThread::Create(DBusThread::BusInterfacePtr bus) {
  SystemInterfacePtr system = make_shared<detail::System>();
  DBusThreadPtr thread(new DBusThread(bus, system));
  return thread;
}

DBusThread::DBusThreadPtr DBusThread::Create(DBusThread::BusInterfacePtr bus,
                                             DBusThread::SystemInterfacePtr system) {
  DBusThreadPtr thread(new DBusThread(bus, system));
  return thread;
}

void DBusThread::AddCommand(DBusThread::CommandPtr command) {
  lock_guard<mutex> guard(commands_mutex_);

  commands_.push_back(command);
}

void DBusThread::StartLoop() {
  loop_running_ = true;

  while (loop_running_) {
    CommandPtr command = GetCommand();
    command->Execute();

    system_->Sleep(1);
  }
}

void DBusThread::StopLoop() {
  loop_running_ = false;
}

bool DBusThread::IsLoopRunning() {
  return loop_running_;
}

DBusThread::DBusThread(DBusThread::BusInterfacePtr bus,
                       DBusThread::SystemInterfacePtr system)
  : bus_(bus),
  system_(system),
  loop_running_(false) {
}

DBusThread::CommandPtr DBusThread::GetCommand() {
  lock_guard<mutex> guard(commands_mutex_);
  CommandPtr command = commands_.front();
  commands_.pop_front();

  return command;
}

}
