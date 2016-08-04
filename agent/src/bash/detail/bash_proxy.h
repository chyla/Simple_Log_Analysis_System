#pragma once

#include <memory>
#include <string>

#include <patlms/dbus/proxy_object.h>
#include <patlms/dbus/detail/bus_interface.h>
#include <patlms/type/bash_log_entry.h>

namespace bash
{

namespace detail
{

class BashProxy : public dbus::ProxyObject {
 public:
  BashProxy(std::shared_ptr<::dbus::detail::BusInterface> bus);

  bool AddLogEntry(const type::BashLogEntry &log_entry);

 private:
  std::shared_ptr<::dbus::detail::BusInterface> bus_;
};

}

}
